#ifndef TRAINTESTSPLITTER
#define TRAINTESTSPLITTER

#include <vector>
#include <string>
#include <map>

struct QueryRelevance
{
    int queryNo;
    int rNo;
    std::string documentNo;
    //int layer;
    int relevance;

    std::string toString();
};

class TrainTestSplitter
{
public:
    enum class SplitMode
    {
        Random, // n random QRels will assigned to train set, others will be assigned to test set; n = allQRelsCount * splitRatio
        TopN,    // the top n QRels will be assigned to train set, others will be assigned to test set; n = allQRelsCount * splitRatio
        Balanced, // train set will contain: relevantQRelsCount * splitRatio QRels; partialRelevantQRelsCount * splitRatio QRels; etc. 
        SplitByQueryId
    };

    // splitRatio (0.0 - 1.0) - determines size of QRels subcollection, that will be used as train set; size_of_train_set = splitRation * allQRelsCount
    TrainTestSplitter(std::vector<std::string> qrelsFilenames, float splitRatio, SplitMode splitMode, bool shuffleSetBeforeSplit = false);

    const std::vector<QueryRelevance>& getTrainSet() const;

    const std::vector<QueryRelevance>& getTestSet() const;

private:
    void readQRelsFromFile(std::string filename);

    void splitSet();

    void splitToBalancedSet();

    void splitByQueryId();

    // determines if relevance for specified document and query is already assigned to allQRels collection; pair: queryNo, docNo
    std::map<std::pair<int, std::string>, bool> assignedQRelsMap;

    std::vector<QueryRelevance> trainSet;
    std::vector<QueryRelevance> testSet;
    std::vector<QueryRelevance> allQRels;

    float splitRatio;
    SplitMode splitMode;
};

#endif // !TRAINTESTSPLITTER