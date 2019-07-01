#include "../headers/TrainTestSplitter/traintestsplitter.h"
#include <iostream>

int main(int argc, char* argv[])
{
    TrainTestSplitter splitter({"data/qrels-final-abstracts.txt", "data/qrels-treceval-abstracts-2018-v2.txt"}, 0.925, TrainTestSplitter::SplitMode::Balanced, true);

    auto train = splitter.getTrainSet();
    auto test = splitter.getTestSet();

    std::cout<< "Train Set: " <<std::endl;
    for(int i=0; i < 10 && i < train.size() ; i++)
    {
        std::cout<< train[i].toString() << std::endl;
    }

    std::cout<< "Test Set: " <<std::endl;
    for(int i=0; i < 10 && i < test.size() ; i++)
    {
        std::cout<< test[i].toString() << std::endl;
    }

    return 0;
}