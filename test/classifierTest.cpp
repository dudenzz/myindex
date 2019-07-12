#include "../headers/Classifier/liblinearclassifier.h"
#include "../headers/TrainTestSplitter/traintestsplitter.h"
#include "../headers/Preprocessing/index.h"
#include <iostream>
#include <memory>
#include <filesystem>

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
    if(!fs::exists(outDir + testOutDirName))
    {
        fs::create_directory(outDir + testOutDirName);
        std::cout << "Test Output dir does not exist. Creating..." << std::endl;
    }
    int processesNo = std::stoi(argv[3]);


    const std::string qRelsFileName = "data/QRels18/qrels-treceval-abstracts-2018-v2.txt";
    TrainTestSplitter splitter({qRelsFileName}, 0.85, TrainTestSplitter::SplitMode::Balanced, false);
    
    // copy TRAIN SET files
    auto train = splitter.getTrainSet();
    std::cout<<"Copying train set files..."<<std::endl;
    unsigned int copiedCounter = 0;
    unsigned int skippedCounter = 0;
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
                skippedCounter++;
            }
            else
            {
                fs::copy_file(pathToFile, outFile);
                copiedCounter++;
            }
        }
        else
        {
            //std::cout << "File for document id: " + qrel.documentNo + " does not exist." << std::endl;
            notExistingCounter++;
        }
    }
    std::cout<< "Copied " + std::to_string(copiedCounter) + " files." << std::endl;
    std::cout<< "Skipped " + std::to_string(skippedCounter) + " files." << std::endl;
    std::cout<< "Not found " + std::to_string(notExistingCounter) + " files." << std::endl;
    std::cout<< std::string(80, '-') << std::endl;


    // copy TEST SET files
    auto test = splitter.getTestSet();
    std::cout<<"Copying test set files..."<<std::endl;
    copiedCounter = 0;
    skippedCounter = 0;
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
                skippedCounter++;
            }
            else
            {
                fs::copy_file(pathToFile, outFile);
                copiedCounter++;
            }
        }
        else
        {
            //std::cout << "File for document id: " + qrel.documentNo + " does not exist." << std::endl;
            notExistingCounter++;
        }
    }
    std::cout<< "Copied " + std::to_string(copiedCounter) + " files." << std::endl;
    std::cout<< "Skipped " + std::to_string(skippedCounter) + " files." << std::endl;
    std::cout<< "Not found " + std::to_string(notExistingCounter) + " files." << std::endl;
    std::cout<< std::string(80, '-') << std::endl;


    // create wordmap
    Index c = Index(outDir, processesNo);
	c.AssignDocuments();
	c.CreateWordmap();
	c.TrimWordmap();
	c.GetWordmap().SaveWordmap(outDir+"train_test_set_wordmap.txt");

    return 2;

    // tokenize

    // create train file

    // create test file

    // train
    std::shared_ptr<BinaryClassifier> classifier = std::make_shared<LibLinearClassifier>();
    classifier->train("trainfile");

    // test
    bool prediction = classifier->predict("testfile");
    std::cout << "Prediction: " << prediction << std::endl;

    // predict
    return 0;
}