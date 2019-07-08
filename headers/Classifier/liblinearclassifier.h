#ifndef LIBLINEARCLASSIFIER
#define LIBLINEARCLASSIFIER

#include <string>
#include "binaryclassifier.h"

class LibLinearClassifier : public BinaryClassifier
{
public:
    LibLinearClassifier();

    void train();

    bool predict(std::string filename);

private:
    const std::string model_filename = "data/liblinear_model";
    const std::string training_set_filename = "liblinear/heart_scale";
};

#endif // !LIBLINEARCLASSIFIER
