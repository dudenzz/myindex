#include <list>
#include <string>

//abstract tokenizer class
class Tokenizer
{
	public:
		char name;
		virtual std::list<std::string> _word_tokenize_simple(std::string text) = 0;
};

