#include <iostream>
#include <fstream>
#include "../headers/Preprocessing/punktpp.h"

int main(int argc, char** argv)
{
	if(argc != 2)
	{
		std::cout<<"Wrong params"<<std::endl;
	}

	std::ifstream strm(argv[1]);
	PunktTokenizer tok = PunktTokenizer();
	std::string text;
	std::string all_text = "";
	while(strm)
	{
		strm >> text;
		all_text += text + " ";
	}
	auto tokens = tok._word_tokenize_simple(all_text);
	for(auto& t : tokens)
	{
		std::cout<<t<<std::endl;
	}
	std::cout << std::endl << all_text << std::endl;
}
