#include "../../headers/Preprocessing/indexed_document.h"
#include <string>

std::string IndexedDocument::to_string()
{
	std::string c = "Document start : {\n";
	for(auto token : values)
	{
		c = c+ std::to_string(token.first) + " : [";
		for (auto position : token.second)
			{
				c = c+ std::to_string(position) + ","; 
			}
		c = c + "]\n";
	}
	c = c+"}\n";
	return c;
}

void IndexedDocument::add_token(int index, int position)
{
     values[index].push_back(position);
}

IndexedDocument::IndexedDocument()
{

}