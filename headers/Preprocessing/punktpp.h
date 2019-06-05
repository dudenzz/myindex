#include "tokenizer.h"
//TODO - a lot
class PunktTokenizer : public Tokenizer
{
	public:
		//name of a tokenizer
		char name;
		//a simple tokenizing method
		std::list<std::string> _word_tokenize_simple(std::string text);
};
