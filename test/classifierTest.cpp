#include "../headers/Classifier/liblinearclassifier.h"
#include <iostream>
#include <memory>

int main(int argc, char* argv[])
{
    std::shared_ptr<BinaryClassifier> classifier = std::make_shared<LibLinearClassifier>();
    classifier->train();

    bool prediction = classifier->predict("liblinear/h1");
    std::cout << "Prediction: " << prediction << std::endl;
    return 0;
}