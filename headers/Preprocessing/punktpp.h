#ifndef PUNKTPP_H
#define PUNKTPP_H

#include "tokenizer.h"
#include <vector>
//TODO - a lot
class PunktTokenizer : public Tokenizer
{
	public:
		//name of a tokenizer
		char name;
		//a simple tokenizing method
		std::list<std::string> _word_tokenize_simple(std::string text);
		std::list<std::pair<std::string,std::string>> _collocations(std::vector<std::string> col_tokens, std::list<std::string> tokens);
};

#endif // !PUNKTPP_H
