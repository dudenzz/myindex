#include "../headers/Classifier/liblinearclassifier.h"
#include "../headers/TrainTestSplitter/traintestsplitter.h"
#include "../headers/Preprocessing/index.h"
#include "../headers/Preprocessing/punktpp.h"
#include <iostream>
#include <memory>
#include <filesystem>
#include <fstream>
#include <algorithm>

namespace fs = std::filesystem;

std::string docIdToFilename(std::string docId)
{
    if(docId.size()>3)
    {
        std::string result = "";
        return result + docId[0] + "/" + docId[1] + "/" + docId[2] + "/" + docId;
    }
    return "";
}

int main(int argc, char* argv[])
{
    // parse parameters
    if(argc != 4) {
        std::cout << "Wrong parameters number (not 4)!"<<std::endl;
        return 1;
    }
    std::string inDir(argv[1]);
    if(inDir[inDir.size()-1] != '/') inDir = inDir + "/";
    if(!fs::exists(inDir))
    {
        std::cout << "Input dir does not exist. Aborting..." << std::endl;
        return 1;
    }
    std::string outDir(argv[2]);
    if(outDir[outDir.size()-1] != '/') outDir = outDir + "/";
    if(!fs::exists(outDir))
    {
        fs::create_directory(outDir);
        std::cout << "Output dir does not exist. Creating..." << std::endl;
    }
    std::string trainOutDirName = "train_set/";
    std::string testOutDirName = "test_set/";
    if(!fs::exists(outDir + trainOutDirName))
    {
        fs::create_directory(outDir + trainOutDirName);
        std::cout << "Train Output dir does not exist. Creating..." << std::endl;
    }
    else
    {
        std::cout<<"Clearing train output dir." << std::endl;
        std::cout << "Deleted elements: " << fs::remove_all(outDir + trainOutDirName) << std::endl;
        fs::create_directory(outDir + trainOutDirName);
    }
    if(!fs::exists(outDir + testOutDirName))
    {
        fs::create_directory(outDir + testOutDirName);
        std::cout << "Test Output dir does not exist. Creating..." << std::endl;
    }
    else
    {
        std::cout<<"Clearing test output dir." << std::endl;
        std::cout << "Deleted elements: " << fs::remove_all(outDir + testOutDirName) << std::endl;
        fs::create_directory(outDir + testOutDirName);
    }
    int processesNo = std::stoi(argv[3]);


    const std::string qRelsFileName = "data/QRels18/qrels-treceval-abstracts-2018-v2.txt";
    TrainTestSplitter splitter({qRelsFileName}, 0.90, TrainTestSplitter::SplitMode::Balanced, true);
    std::map<std::string, int> docIdToRelevance;

    // copy TRAIN SET files
    auto train = splitter.getTrainSet();
    std::cout<<"Copying train set files..."<<std::endl;
    unsigned int copiedCounter = 0;
    unsigned int alreadyCopied = 0;
    unsigned int notExistingCounter = 0;
    for(auto& qrel : train)
    {
        const std::string pathToFile = inDir + docIdToFilename(qrel.documentNo) + ".xml";
        if(fs::exists(pathToFile))
        {
            std::string outFile = outDir + trainOutDirName + qrel.documentNo + ".xml";
            if(fs::exists(outFile))
            {
                //std::cout<<"File " + qrel.documentNo + ".xml" + " already copied. Skipping." <<std::endl;
                alreadyCopied++;
                docIdToRelevance[qrel.documentNo] = qrel.relevance;
            }
            else
            {
                fs::copy_file(pathToFile, outFile);
                copiedCounter++;
                docIdToRelevance[qrel.documentNo] = qrel.relevance;
            }
        }
        else
        {
            //std::cout << "File for document id: " + qrel.documentNo + " does not exist." << std::endl;
            notExistingCounter++;
        }
    }
    std::cout<< "Copied " + std::to_string(copiedCounter) + " files." << std::endl;
    std::cout<< "Already copied " + std::to_string(alreadyCopied) + " files." << std::endl;
    std::cout<< "Not found " + std::to_string(notExistingCounter) + " files." << std::endl;
    std::cout<< std::string(80, '-') << std::endl;


    // copy TEST SET files
    auto test = splitter.getTestSet();
    std::cout<<"Copying test set files..."<<std::endl;
    copiedCounter = 0;
    alreadyCopied = 0;
    notExistingCounter = 0;
    for(auto& qrel : test)
    {
        const std::string pathToFile = inDir + docIdToFilename(qrel.documentNo) + ".xml";
        if(fs::exists(pathToFile))
        {
            std::string outFile = outDir + testOutDirName + qrel.documentNo + ".xml";
            if(fs::exists(outFile))
            {
                //std::cout<<"File " + qrel.documentNo + ".xml" + " already copied. Skipping." <<std::endl;
                alreadyCopied++;
                docIdToRelevance[qrel.documentNo] = qrel.relevance;
            }
            else
            {
                fs::copy_file(pathToFile, outFile);
                copiedCounter++;
                docIdToRelevance[qrel.documentNo] = qrel.relevance;
            }
        }
        else
        {
            //std::cout << "File for document id: " + qrel.documentNo + " does not exist." << std::endl;
            notExistingCounter++;
        }
    }
    std::cout<< "Copied " + std::to_string(copiedCounter) + " files." << std::endl;
    std::cout<< "Already copied " + std::to_string(alreadyCopied) + " files." << std::endl;
    std::cout<< "Not found " + std::to_string(notExistingCounter) + " files." << std::endl;
    std::cout<< std::string(80, '-') << std::endl;


    // create wordmap
    std::cout << "Creating wordmap..." <<std::endl;
    Index c = Index(outDir, processesNo);
	c.AssignDocuments();
	c.CreateWordmap();
	c.TrimWordmap();
	c.GetWordmap().SaveWordmap(outDir+"train_test_set_wordmap.txt");
    auto wordmap = c.GetWordmap();


    PunktTokenizer tok = PunktTokenizer();

    // create train file
    std::cout << "Creating train file..." <<std::endl;
    const std::string trainSetFileName = outDir + "train_set_for_liblinear.txt";
    std::ofstream trainSetFile(trainSetFileName);
    for(auto& file: fs::recursive_directory_iterator(outDir + trainOutDirName))
    {
        std::string text;
        std::string all_text = "";
        std::string filename = file.path().filename();
        std::ifstream strm(file.path());
        while(strm)
        {
            strm >> text;
            all_text += text + " ";
        }
        auto tokens = tok._word_tokenize_simple(all_text);
        std::map<int, bool> usedTokens;

        // remove '.xml' to get doc id only (from filename
        std::string docId = std::string(filename.begin(), filename.end()-4); 
        std::string relevance;
        if(docIdToRelevance.find(docId) != docIdToRelevance.end())
        {
            relevance = (docIdToRelevance[docId] == 0 ? "-1" : "+1");
        }
        else
        {
            std::cout << "Cannot find relevance for given doc id. "<<std::endl;
            continue;
        }

        // write to file
        std::vector<std::pair<int,int>> samples;
        trainSetFile << relevance;;
        auto& wordToWordIdIndex = wordmap.GetWordIndex();
        for(auto& token : tokens)
        {
            int wordId = wordToWordIdIndex[token];
            if(usedTokens.find(wordId) == usedTokens.end() && wordId != 0) // there is no given id in the set, so write it to the file
            {
                samples.push_back(std::pair<int,int>(wordId, 1));
                usedTokens[wordId] = 1;
            }  
        }
        std::sort(samples.begin(), samples.end(), [](std::pair<int,int>& a, std::pair<int, int>& b){return a.first < b.first; });
        for(auto& p : samples)
        {
            trainSetFile <<" "<<p.first << ":" << p.second;
        }
        trainSetFile << std::endl;
    }
    trainSetFile.close();


    // create test file
    std::cout << "Creating test file..." <<std::endl;
    const std::string testSetFileName = outDir + "test_set_for_liblinear.txt";
    std::ofstream testSetFile(testSetFileName);
    for(auto& file: fs::recursive_directory_iterator(outDir + testOutDirName))
    {
        std::string text;
        std::string all_text = "";
        std::string filename = file.path().filename();
        std::ifstream strm(file.path());
        while(strm)
        {
            strm >> text;
            all_text += text + " ";
        }
        auto tokens = tok._word_tokenize_simple(all_text);
        std::map<int, bool> usedTokens;

        // remove '.xml' to get doc id only (from filename
        std::string docId = std::string(filename.begin(), filename.end()-4); 
        std::string relevance;
        if(docIdToRelevance.find(docId) != docIdToRelevance.end())
        {
            relevance = (docIdToRelevance[docId] == 0 ? "-1" : "+1");
        }
        else
        {
            std::cout << "Cannot find relevance for given doc id. "<<std::endl;
            continue;
        }

        // write to file
        testSetFile << relevance;
        auto& wordToWordIdIndex = wordmap.GetWordIndex();
        std::vector<std::pair<int, int>> samples;
        for(auto& token : tokens)
        {
            int wordId = wordToWordIdIndex[token];
            if(usedTokens.find(wordId) == usedTokens.end() && wordId != 0) // there is no given id in the set, so write it to the file
            {
                samples.push_back(std::pair<int,int>(wordId, 1));
                usedTokens[wordId] = 1;
            }  
        }
        std::sort(samples.begin(), samples.end(), [](std::pair<int,int>& a, std::pair<int, int>& b){return a.first < b.first; });
        for(auto& p : samples)
        {
            testSetFile <<" "<<p.first << ":" << p.second;
        }
        testSetFile << std::endl;
    }
    testSetFile.close();


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