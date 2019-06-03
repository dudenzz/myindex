#include "wordmap.h"
#include "helpers.h"
#include <string>
#include <algorithm>
#include <fstream>
#include <iostream>
Wordmap::Wordmap()
{
	id_generator = 0;
	no_docs = 0;
	no_toks = 0;
}

void Wordmap::SaveWordmap(std::string filename)
{
	std::ofstream fileOut;
	fileOut.open(filename);
	fileOut << no_docs << " " << no_toks << std::endl;
	for(const auto& kv: word_index)
	{
		std::string word = kv.first;
		int index = kv.second;
		int cf = word_cf[index];
		int df = word_df[index];
		fileOut << word << " " << index << " " << df << " " << cf <<std::endl;
	}
	fileOut.close();

}
void Wordmap::Join(Wordmap nmap)
{
	this->no_docs += nmap.no_docs;
	this->no_toks += nmap.no_toks;
	for(const auto& kv: nmap.word_index)
	{
		std::string word = kv.first;
		std::pair<std::map<std::string,int>::iterator,bool> ret;
		ret = word_index.insert(std::pair<std::string, int>(word, id_generator));
		if(ret.second == false)
		{
			int id = word_index[word];
			int map_id = nmap.word_index[word];
			word_cf[id] += nmap.word_cf[map_id];
			word_df[id] += nmap.word_df[map_id];
		}
		else
		{
			int map_id = nmap.word_index[word];
	                reverse_index.insert(std::pair<int,std::string>(id_generator,word));
			word_cf.insert(std::pair<int,int>(id_generator,nmap.word_cf[map_id]));
			word_df.insert(std::pair<int,int>(id_generator,nmap.word_df[map_id]));
			id_generator += 1;
		}
	}
}
int Wordmap::AddWord(std::string word, std::list<std::string>* added_in_doc)
{
	this->no_toks += 1;
	std::pair<std::map<std::string,int>::iterator,bool> ret;
	ret = word_index.insert(std::pair<std::string, int>(word, id_generator));
	if(ret.second == false)
	{
		std::map<std::string,int>::iterator find_index = word_index.find(word);
		int id = find_index->second;
		std::map<int,int>::iterator find_cf = word_cf.find(id);
		find_cf->second += 1;
		bool result = helpers::contains(*added_in_doc, word);
		if(!result)
		{
			added_in_doc->push_back(word);
	                std::map<int,int>::iterator find_df = word_df.find(id);
	                find_df->second += 1;
		}
	}
	else
	{
		reverse_index.insert(std::pair<int,std::string>(id_generator,word));
		word_cf.insert(std::pair<int,int>(id_generator,1));
		word_df.insert(std::pair<int,int>(id_generator,1));
		id_generator += 1;
	}
	return 0;
}
int Wordmap::AddDocument(std::list<std::string> tokens)
{
	this->no_docs +=1;
	std::list<std::string> added_in_doc;
	for(auto token : tokens)
	{
		AddWord(token, &added_in_doc);
	}
	return 0;
}

void Wordmap::MinCount(int min_count)
{
	std::list<int> toDelete;
	for(const auto& kv : word_index)
	{
		int id = kv.second;
		if(word_cf[id] < min_count)
			toDelete.push_back(id);
	}
	for(int i : toDelete)
	{
		word_df.erase(i);
		word_cf.erase(i);
		std::string word = reverse_index[i];
		reverse_index.erase(i);
		word_index.erase(word);
	}
}
