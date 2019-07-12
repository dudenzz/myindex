#ifndef LIBLINEARCLASSIFIER
#define LIBLINEARCLASSIFIER

#include <string>
#include "binaryclassifier.h"

class LibLinearClassifier : public BinaryClassifier
{
public:
    LibLinearClassifier();

    void train(std::string filename);

    bool predict(std::string filename);

private:
    const std::string model_filename = "data/liblinear_model";
};

#endif // !LIBLINEARCLASSIFIER
