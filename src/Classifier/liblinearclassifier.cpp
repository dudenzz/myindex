#include <memory>
#include <vector>
#include <iostream>
#include <fstream>

#include "../../headers/Classifier/liblinearclassifier.h"

LibLinearClassifier::LibLinearClassifier() 
{

}

void LibLinearClassifier::train(std::string filename)
{
    std::string flags = "";
    std::string command = "liblinear/train";

    command += " " + flags;
    command += " " + filename;
    command += " " + model_filename;

    std::cout << "BinaryClassifier | Running command: " << command << std::endl;
    int ret = system(command.c_str());
    std::cout << "BinaryClassifier | Subprogram liblinear/train returned: " << ret << std::endl;
}

bool LibLinearClassifier::predict(std::string test_filename)
{
    std::string flags = "";
    std::string output_filename = "predict_out";

    std::string command = "liblinear/predict";
    command += " " + flags;
    command += " " + test_filename;
    command += " " + model_filename;
    command += " " + output_filename; 

    std::cout << "BinaryClassifier | Running command: " << command << std::endl;
    int ret = system(command.c_str());
    std::cout << "BinaryClassifier | Subprogram liblinear/predict returned: " << ret << std::endl;

    std::ifstream in("predict_out");
    int result;
    in >> result;

    return result;
}
