#include "../../headers/Preprocessing/punktpp.h"
#include "../../headers/helpers/helpers.h"

//a basic, simple tokenizing method
std::list<std::string> PunktTokenizer::_word_tokenize_simple(std::string text)
{
	std::list<std::string> list_of_tokens;
	std::string current_token = "";
	//process the document character by character
	for(char &c : text)
	{	
		//TODO - put the specific characters into properties file
		//space and tabulator are basic delimeters and are ommited, once you encounter space, save the current token and start a new one
		if (c == ' ' || c == '\t')
		{
			if(current_token != "") list_of_tokens.push_back(current_token);
			current_token = "";
		}
		else
		{
			//if you encounter one of the special characters, save the current token, save a token with special character and start a new one
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
				//if you encounter one of the digits, save the current token, save a digit token and start a new one
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
					//if you encounter a newline, save the current token, save a newline token and start a new one
					if (c == '\n')
					{
						if(current_token != "") list_of_tokens.push_back(current_token);
						list_of_tokens.push_back("<NewLine>");
						current_token = "";
					}
					//otherwise append current chracter to a current token
					else
					{
						//if a character is a large letter, lowercase it
						if((int)c  > 64 && (int)c < 91)
							c += 32;
						current_token += c;
					}
				}
			}
		}
	}
	//add the token at EOF
	if(current_token != "" 
		&& trim_copy(list_of_tokens.back()) != trim_copy(current_token)) 
		list_of_tokens.push_back(current_token);
	//return the list of tokens
	return list_of_tokens;
}
std::list<std::pair<std::string,std::string>> PunktTokenizer::_collocations(std::vector<std::string> col_tokens, std::list<std::string> tokens)
{
	//for token in tokens:
		//if next_col:
			//collocation = (prev_token, token)
			//next_col = false
		//if token in col_tokens:
			//if prev_token:
				//collocation = (prev_token, token)
			//next_col = true
		//prev_token = token
	std::list<std::pair<std::string, std::string>> cols;
	int next_col = 0;
	std::string prev_token = "";
	for(std::string token : tokens)
	{
		if(next_col = 1)
		{
			cols.push_back(std::make_pair(prev_token, token));
			next_col = 0;
		}
		if(std::find(col_tokens.begin(),col_tokens.end(),token) != col_tokens.end())
		{
			next_col = 1;
			if(prev_token.compare("") != 0)
			      cols.push_back(std::make_pair(prev_token, token));

		}
		prev_token = token;
	}
	return cols;
}
