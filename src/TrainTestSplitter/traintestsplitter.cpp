#include "../../headers/TrainTestSplitter/traintestsplitter.h"
#include <fstream>
#include <iostream>

std::string QueryRelevance::toString()
{
    return "QueryNo: " + std::to_string(queryNo) + ", rNo: " + std::to_string(rNo) + ", DocNo: " + documentNo + ", Relevance: " + std::to_string(relevance) + "."; 
}

TrainTestSplitter::TrainTestSplitter(std::vector<std::string> qRelsFilenames, float splitRatio_, SplitMode splitMode_) 
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
    int trainSetSize = static_cast<double>(allQRels.size()) * splitRatio;
    std::cout << "Train set will contain " + std::to_string(trainSetSize) + " QRels, test set will contain " + std::to_string(allQRels.size() - trainSetSize) + " QRels." << std::endl;
    std::cout<<"Splitting QRels to train and test set... "<<std::endl;
   
    switch(splitMode)
    {
    case TrainTestSplitter::SplitMode::TopN:
        trainSet = std::vector<QueryRelevance>(allQRels.begin(), allQRels.begin() + trainSetSize);
        testSet = std::vector<QueryRelevance>(allQRels.begin() + trainSetSize, allQRels.end());
        break;

    case TrainTestSplitter::SplitMode::Random:
        // TODO
        break;
    }

    // it won't be used anymore
    allQRels.clear();
}
