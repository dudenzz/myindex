#include "../headers/Classifier/liblinearclassifier.h"
#include "../headers/TrainTestSplitter/traintestsplitter.h"
#include "../headers/Preprocessing/index.h"
#include "../headers/Preprocessing/punktpp.h"
#include <iostream>
#include <memory>
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/erase.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <set>

namespace fs = std::filesystem;

int main(int argc, char* argv[])
{
    // parse parameters
    if(argc != 4) {
        std::cout << "Wrong parameters number (not 3)! Current: " << argc-1 <<std::endl; // 0 arg = program name
        return 1;
    }
    std::string inDir(argv[1]);
    if(inDir[inDir.size()-1] != '/') inDir = inDir + "/";
    if(!fs::exists(inDir))
    {
        std::cout << "Input dir does not exist. Aborting..." << std::endl;
        return 1;
    }
    std::string baseOutDir(argv[2]);
    if(baseOutDir[baseOutDir.size()-1] != '/') baseOutDir = baseOutDir + "/";
    if(!fs::exists(baseOutDir))
    {
        fs::create_directory(baseOutDir);
        std::cout << "Output dir does not exist. Creating..." << std::endl;
    }
    std::string trainOutDirName = "train_set/";
    std::string testOutDirName = "test_set/";
    if(!fs::exists(baseOutDir + trainOutDirName))
    {
        fs::create_directory(baseOutDir + trainOutDirName);
        std::cout << "Train Output dir does not exist. Creating..." << std::endl;
    }
    else
    {
        std::cout<<"Clearing train output dir." << std::endl;
        std::cout << "Deleted elements: " << fs::remove_all(baseOutDir + trainOutDirName) << std::endl;
        fs::create_directory(baseOutDir + trainOutDirName);
    }
    if(!fs::exists(baseOutDir + testOutDirName))
    {
        fs::create_directory(baseOutDir + testOutDirName);
        std::cout << "Test Output dir does not exist. Creating..." << std::endl;
    }
    else
    {
        std::cout<<"Clearing test output dir." << std::endl;
        std::cout << "Deleted elements: " << fs::remove_all(baseOutDir + testOutDirName) << std::endl;
        fs::create_directory(baseOutDir + testOutDirName);
    }
    int processesNo = std::stoi(argv[3]);

    const std::string qRelsFileName = "data/QRels18/qrels-treceval-abstracts-2018-v2.txt";
    TrainTestSplitter splitter({qRelsFileName}, 0.97, TrainTestSplitter::SplitMode::SplitByQueryId, false);
    std::map<std::string, int> docIdToRelevance;

    auto const& trainSet = splitter.getTrainSet();
    auto const& testSet = splitter.getTestSet();

    std::cout << "Train set contains " + std::to_string(trainSet.size()) + " QRels." << std::endl;
    std::cout << "Test set contains " + std::to_string(testSet.size()) + " QRels." << std::endl;

    // for log only
    std::set<int> trainQueries;
    std::set<int> testQueries;

    std::set<std::string> trainDocs;

    for(auto& qrel : trainSet)
    {
        trainDocs.insert(qrel.documentNo);
        trainQueries.insert(qrel.queryNo);
        docIdToRelevance[qrel.documentNo] = qrel.relevance;
    }

    std::set<std::string> testDocs;
    for(auto& qrel : testSet)
    {
        testDocs.insert(qrel.documentNo);
        testQueries.insert(qrel.queryNo);
        docIdToRelevance[qrel.documentNo] = qrel.relevance;
    }

    std::cout << "Train queries: ";
    for(auto& a : trainQueries) std::cout << std::to_string(a) << " ";
    std::cout << std::endl;
    std::cout << "Test queries: ";
    for(auto& a : testQueries) std::cout << std::to_string(a) << " ";
    std::cout << std::endl;

    // create wordmap
    std::cout << "Creating wordmap..." <<std::endl;
    Index c = Index(inDir, processesNo);
	c.AssignDocuments();
	c.CreateWordmap();
	c.TrimWordmap();
	//c.GetWordmap().SaveWordmap(baseOutDir + "train_test_set_wordmap.txt");
    auto wordmap = c.GetWordmap();

    PunktTokenizer tok = PunktTokenizer();

    // create train & test files
    std::cout << "Creating train and test files..." <<std::endl;
    const std::string trainSetFileName = baseOutDir + "train_set_for_liblinear.txt";
    const std::string testSetFileName = baseOutDir + "test_set_for_liblinear.txt";

    // clear existing files
    std::ofstream truncStrm;
    truncStrm.open(trainSetFileName, std::ofstream::out | std::ofstream::trunc);
    truncStrm.close();
    truncStrm.open(testSetFileName, std::ofstream::out | std::ofstream::trunc);
    truncStrm.close();

    for(auto& file: fs::recursive_directory_iterator(inDir))
    {
        std::string filename = file.path().filename();
        std::string docId = filename;
        // remove '.xml' to get doc id only (from filename)
        boost::erase_all(docId, ".xml");

        std::ofstream fileStream;
        if(trainDocs.find(docId) != trainDocs.end())
        {
            fileStream.open(trainSetFileName, std::ofstream::out | std::ofstream::app);
        }
        else if(testDocs.find(docId) != testDocs.end())
        {
            fileStream.open(testSetFileName, std::ofstream::out | std::ofstream::app);
        }
        else 
        {
            std::cout<<"DocId " + docId + " does not exist in train and test set." << std::endl;
            continue;
        }

        std::string text;
        std::string all_text = "";

        std::ifstream strm(file.path());
        while(strm)
        {
            strm >> text;
            all_text += text + " ";
        }
        auto tokens = tok._word_tokenize_simple(all_text);
        std::map<int, bool> usedTokens;

        std::string relevance;
        if(docIdToRelevance.find(docId) != docIdToRelevance.end())
        {
            relevance = (docIdToRelevance[docId] == 0 ? "0" : "1");
        }
        else
        {
            std::cout << "Cannot find relevance for given doc id: " << docId <<std::endl;
            continue;
        }

        // write to file
        std::vector<std::pair<int,int>> samples;
        fileStream << relevance;
        auto& wordToWordIdIndex = wordmap.GetWordIndex();
        for(auto& token : tokens)
        {
            int wordId = wordToWordIdIndex[token] + 1;
            if(usedTokens.find(wordId) == usedTokens.end()) // there is no given id in the set, so write it to the file
            {
                samples.push_back(std::pair<int,int>(wordId, 1));
                usedTokens[wordId] = 1;
            }  
        }
        std::sort(samples.begin(), samples.end(), [](std::pair<int,int>& a, std::pair<int, int>& b){return a.first < b.first; });
        for(auto& p : samples)
        {
            fileStream <<" "<<p.first << ":" << p.second;
        }
        fileStream << std::endl;
        fileStream.close();
    }

    // train
    std::cout << "Training classifier..." <<std::endl;
    std::shared_ptr<BinaryClassifier> classifier = std::make_shared<LibLinearClassifier>();
    classifier->train(trainSetFileName);


    // test
    std::cout << "Testing classifier..." <<std::endl;
    classifier->predict(testSetFileName);


    // predict
    return 0;
}