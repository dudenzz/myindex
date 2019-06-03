#include "punktpp.h"
#include "helpers.h"

std::list<std::string> PunktTokenizer::_word_tokenize_simple(std::string text)
{
	std::list<std::string> list_of_tokens;
	std::string current_token = "";
	for(char &c : text)
	{
		if (c == ' ' || c == '\t')
		{
			if(current_token != "") list_of_tokens.push_back(current_token);
			current_token = "";
		}
		else
		{
			if(c == '.' || c == '?' || c == '!' || c == ';' || c == '(' || c == '#'
				|| c == ')' || c == '[' || c == ']' || c == '{' || c == '}'
				|| c == '/' || c == '\\' || c == ':' || c == '&' || c == '\'' || c == '`' || c == '~'
				|| c == '"' || c == '|' || c == '<' || c == '>' || c == ',' || c == '^' || c == '_' 
				|| c == '$' || c == '%' || c == '*' || c == '+' || c == '-' || c=='=' || c=='@')
			{
				if(current_token != "") list_of_tokens.push_back(current_token);
				std::string tmp = "";
				tmp += c;
				list_of_tokens.push_back(tmp);
				current_token = "";
			}
			else
			{
				if (c > 47 && c < 58)
				{
	                                if(current_token != "") list_of_tokens.push_back(current_token);
					std::string stp = "<Digit";
					stp.push_back(c);
					stp += ">";
					list_of_tokens.push_back(stp);
                                        current_token = "";

				}
				else
				{
					if (c == '\n')
					{
						if(current_token != "") list_of_tokens.push_back(current_token);
						list_of_tokens.push_back("<NewLine>");
						current_token = "";
					}
					else
					{
						if((int)c  > 64 && (int)c < 91)
							c += 32;
						current_token += c;
					}
				}
			}
		}
	}
	if(current_token != "" 
		&& trim_copy(list_of_tokens.back()) != trim_copy(current_token)) 
		list_of_tokens.push_back(current_token);
	return list_of_tokens;
}
