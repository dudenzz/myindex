#include "../../headers/TrainTestSplitter/traintestsplitter.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <memory>
#include <chrono>
#include <random>

std::string QueryRelevance::toString()
{
    return "QueryNo: " + std::to_string(queryNo) + ", rNo: " + std::to_string(rNo) + ", DocNo: " + documentNo + ", Relevance: " + std::to_string(relevance) + "."; 
}

TrainTestSplitter::TrainTestSplitter(std::vector<std::string> qRelsFilenames, float splitRatio_, SplitMode splitMode_, bool shuffleSetBeforeSplit) 
    : splitRatio(splitRatio_), splitMode(splitMode_) // initialization list
{
    if(splitRatio > 1.0) 
    {
        std::cout << "WARNING! Split ratio cannot be greather or equal to 1.0!" << std::endl;
        splitRatio = 1.0;
    }
    if(splitRatio < 0.0)
    { 
        std::cout << "WARNING! Split ratio cannot be less or equal to 0.0!" << std::endl;
        splitRatio = 0.0;
    }
    //std::cout<<"Reading QRels files... "<<std::endl;
    for(auto& f : qRelsFilenames)
    readQRelsFromFile(f);
    std::cout<<"Total assigned QRels from all files: " + std::to_string(allQRels.size()) << "." << std::endl;

    if(shuffleSetBeforeSplit)
    {
        constexpr unsigned int userSeed = 0; // TYPE HERE USER SEED

        std::shared_ptr<std::default_random_engine> rng;
        std::cout<< std::string(80, '-') << std::endl;
        if(userSeed == 0) // if no USER seed given, use system seed
        {
            unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
            rng = std::make_shared<std::default_random_engine>(seed);
            std::cout << "Shuffling QRels set with SYSTEM seed: " + std::to_string(seed) << "." << std::endl;
        }
        else
        {
            rng = std::make_shared<std::default_random_engine>(userSeed);
            std::cout << "Shuffling QRels set with USER seed: " + std::to_string(userSeed) << "." << std::endl;
        }
        
        std::shuffle(allQRels.begin(), allQRels.end(), *rng);
    }

    splitSet();
}

const std::vector<QueryRelevance>& TrainTestSplitter::getTrainSet() const
{
    return trainSet;
}

const std::vector<QueryRelevance>& TrainTestSplitter::getTestSet() const
{
    return testSet;
}

void TrainTestSplitter::readQRelsFromFile(std::string filename)
{
    int totalQRels = 0;
    int judgedQRels = 0;
    int duplicateCount = 0;
    std::ifstream qRelsStream;
    qRelsStream.open(filename);
    if(!qRelsStream)
    {
        std::cout << "Cannot open QRels file: '" + filename + "'. Skipping." <<std::endl;
    }
    else
    {
         std::cout << "Reading QRels file: '" + filename + "'." <<std::endl;
    }
    

    while(!qRelsStream.eof())
    {
        QueryRelevance tempQRel;
        qRelsStream >> tempQRel.queryNo;
        qRelsStream >> tempQRel.rNo;
        qRelsStream >> tempQRel.documentNo;
        //qRelsStream >> tempQRel.layer;
        qRelsStream >> tempQRel.relevance;
        totalQRels++;

        if(tempQRel.relevance != -1)
        {
            judgedQRels++;
            std::pair<int, std::string> queryToDoc = std::pair<int, std::string>(tempQRel.queryNo, tempQRel.documentNo);
            if(assignedQRelsMap.find(queryToDoc) == assignedQRelsMap.end()) // if queryToDoc does not exist in map
            {
                assignedQRelsMap[queryToDoc] = true;
                allQRels.push_back(tempQRel); // this collection will be used to split set
            }
            else
            {
                duplicateCount++;
            }
        }
    }
    std::cout<<"File contains total QRels: " + std::to_string(totalQRels) + ", judged QRels: " + std::to_string(judgedQRels) + ", already assigned QRels: " + std::to_string(duplicateCount) + "." << std::endl; 
}

void TrainTestSplitter::splitSet()
{
    std::cout<< std::string(80, '-') << std::endl;

    int trainSetSize = static_cast<double>(allQRels.size()) * splitRatio;
    std::cout << "Train set will contain " + std::to_string(trainSetSize) + " QRels (" + std::to_string(static_cast<int>(splitRatio*100)) + "% of all), test set will contain " + std::to_string(allQRels.size() - trainSetSize) + " QRels." << std::endl;
   
    switch(splitMode)
    {
    case TrainTestSplitter::SplitMode::TopN:
        std::cout<<"Splitting QRels to train and test set, method: TOP-N."<<std::endl;
        trainSet = std::vector<QueryRelevance>(allQRels.begin(), allQRels.begin() + trainSetSize);
        testSet = std::vector<QueryRelevance>(allQRels.begin() + trainSetSize, allQRels.end());
        break;

    case TrainTestSplitter::SplitMode::Random:
        // TODO
        break;

    case TrainTestSplitter::SplitMode::Balanced:
        std::cout<<"Splitting QRels to train and test set, method: BALANCED."<<std::endl;
        splitToBalancedSet();
        break;
    }

    // it won't be used anymore
    allQRels.clear();
    std::cout<< std::string(80, '-') << std::endl;
}

void TrainTestSplitter::splitToBalancedSet()
{
    int relevantQRelsCount = 0, partialRelevantQRelsCount = 0, nonRelevantQRelsCount = 0;

    for(auto& qRel : allQRels)
    {
        if(qRel.relevance == 2) relevantQRelsCount++;
        else if(qRel.relevance == 1) partialRelevantQRelsCount++;
        else if(qRel.relevance == 0) nonRelevantQRelsCount++;
    } 

    std::cout << "There are " + std::to_string(relevantQRelsCount) + " relevant QRels, " + 
        std::to_string(partialRelevantQRelsCount) + " partial relevant QRels, " + 
        std::to_string(nonRelevantQRelsCount) + " non-relevant QRels." << std::endl;

    int relevantQRelsInTrainSet = splitRatio * relevantQRelsCount; 
    int partialRelevantQRelsInTrainSet = splitRatio * partialRelevantQRelsCount; 
    int nonRelevantQRelsInTrainSet = splitRatio * nonRelevantQRelsCount;

    std::cout << "Train set will contain " + std::to_string(relevantQRelsInTrainSet) + " relevant QRels, " + 
        std::to_string(partialRelevantQRelsInTrainSet) + " partial relevant QRels, " + 
        std::to_string(nonRelevantQRelsInTrainSet) + " non-relevant QRels." << std::endl;

    std::cout << "Test set will contain " + std::to_string(relevantQRelsCount - relevantQRelsInTrainSet) + " relevant QRels, " + 
        std::to_string(partialRelevantQRelsCount - partialRelevantQRelsInTrainSet) + " partial relevant QRels, " + 
        std::to_string(nonRelevantQRelsCount - nonRelevantQRelsInTrainSet) + " non-relevant QRels." << std::endl;

    relevantQRelsCount = 0;
    partialRelevantQRelsCount = 0; 
    nonRelevantQRelsCount = 0;

    for(auto& qRel : allQRels)
    {
        if(qRel.relevance == 2)
        {
            if(relevantQRelsCount < relevantQRelsInTrainSet)
            {
                trainSet.push_back(qRel);
            }
            else
            {
                testSet.push_back(qRel);
            }
            relevantQRelsCount++;
        }
        else if(qRel.relevance == 1)
        {
            if(partialRelevantQRelsCount < partialRelevantQRelsInTrainSet)
            {
                trainSet.push_back(qRel);
            }
            else
            {
                testSet.push_back(qRel);
            }
            partialRelevantQRelsCount++;
        }
        if(qRel.relevance == 0)
        {
            if(nonRelevantQRelsCount < nonRelevantQRelsInTrainSet)
            {
                trainSet.push_back(qRel);
            }
            else
            {
                testSet.push_back(qRel);
            }
            nonRelevantQRelsCount++;
        }
    }
}
