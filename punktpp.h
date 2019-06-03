#include "tokenizer.h"

class PunktTokenizer : public Tokenizer
{
	public:
		char name;
		std::list<std::string> _word_tokenize_simple(std::string text);
};
