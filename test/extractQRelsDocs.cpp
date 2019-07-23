#include "../headers/TrainTestSplitter/traintestsplitter.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <set>

#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/erase.hpp>
#include <boost/algorithm/string/predicate.hpp>

namespace fs = std::filesystem;

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        std::cout<< "Wrong number of args!" << std::endl;
    }

    std::string inDir(argv[1]), outDir(argv[2]);
    if(inDir[inDir.size()-1] != '/') inDir = inDir + "/";
    if(outDir[outDir.size()-1] != '/') outDir = outDir + "/";

    const std::string qRelsFileName = "data/QRels18/qrels-treceval-abstracts-2018-v2.txt";
    // don't care about splitter params - we need just all qrels
    TrainTestSplitter splitter({qRelsFileName}, 0.90, TrainTestSplitter::SplitMode::SplitByQueryId, true);
    auto const& trainSet = splitter.getTrainSet();
    auto const& testSet = splitter.getTestSet();
    
    std::set<std::string> qrelsDocIds;
    for(auto& qrel : trainSet) qrelsDocIds.insert(qrel.documentNo);
    for(auto& qrel : testSet) qrelsDocIds.insert(qrel.documentNo);

    std::cout<<"Copying docs..." << std::endl;
    int filesCounter = 0;

    for(auto& file: fs::directory_iterator(inDir))
    {
        if(file.is_regular_file())
        {
            std::ifstream fin(file.path().string());
            if(!fin)
            {
                std::cout<<"ERROR file "<< file.path().string();
                continue;
            }
            std::vector<std::string> documentContent;
            std::string docId;
            while(!fin.eof())
            {
                std::string line;
                std::getline(fin, line);
                documentContent.push_back(line);

                boost::trim(line);
                if(boost::contains(line, "<document>"))
                {
                    if(!docId.empty())
                    {
                        if(qrelsDocIds.find(docId) != qrelsDocIds.end())
                        {
                            std::ofstream stream(outDir + docId + ".xml");
                            if(stream)
                            {
                                for(auto& c : documentContent)
                                stream << c << std::endl;
                            }
                            else
                            {
                                std::cout<< "Stream error, docid " + docId << std::endl;
                            }
                        }
                        documentContent.clear();
                        docId.clear();
                    }
                }

                if(boost::starts_with(line, "<docid>"))
                {
                    boost::erase_all(line, "<docid>");
                    boost::erase_all(line, "</docid>");
                    docId = line;
                }
            }
            std::cout << "Fully parsed files: " << ++filesCounter << std::endl;
        }
    }
    std::cout<<"Finished." << std::endl;
}