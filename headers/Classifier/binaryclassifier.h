#ifndef BINARYCLASSIFIER
#define BINARYCLASSIFIER

class BinaryClassifier 
{
public:
    virtual void train(std::string filename) = 0;

    virtual bool predict(std::string filename) = 0;
};

#endif // !BINARYCLASSIFIER