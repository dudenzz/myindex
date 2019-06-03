#include <iostream>
#include <fstream>
#include "punktpp.h"

int main()
{
	std::ifstream strm("//home/jakub/twe/last_version/20-news-all/alt.atheism/54481");
	PunktTokenizer tok = PunktTokenizer();
	std::string text;
	std::string all_text = "";
	while(strm)
	{
		strm >> text;
		all_text += text + " ";
	}
	auto tokens = tok._word_tokenize_simple(all_text);
	std::cout << all_text << std::endl;
}
