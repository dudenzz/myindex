#include "../../headers/Preprocessing/wordmap.h"
#include "../../headers/helpers/helpers.h"
#include <string>
#include <algorithm>
#include <fstream>
#include <iostream>
//simple constructor
Wordmap::Wordmap()
{
	//id of the first document = 0
	id_generator = 0;
	//number of processed documents = 0
	no_docs = 0;
	//number of processed tokens = 0
	no_toks = 0;
}

//method of saving a wordmap in a text file
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
//joining two wordmaps
void Wordmap::Join(Wordmap nmap)
{
	//total number of processed documents and tokens = current + new
	this->no_docs += nmap.no_docs;
	this->no_toks += nmap.no_toks;
	//for each token in a new wordmap
	for(const auto& kv: nmap.word_index)
	{
		//word is a first element of a pair, index is second, get the word
		std::string word = kv.first;
		//check if the word exists in the original wordmap
		std::pair<std::map<std::string,int>::iterator,bool> ret;
		ret = word_index.insert(std::pair<std::string, int>(word, id_generator));
		//if it does add the statistics to the current word 
		if(ret.second == false)
		{
			int id = word_index[word];
			int map_id = nmap.word_index[word];
			word_cf[id] += nmap.word_cf[map_id];
			word_df[id] += nmap.word_df[map_id];
		}
		//if it doesn't create a new word
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
//adding a single word to a wordmap
int Wordmap::AddWord(std::string word, std::list<std::string>* added_in_doc)
{
	//number of processed words += 1
	this->no_toks += 1;
	//check if the word exists in wordmap
	std::pair<std::map<std::string,int>::iterator,bool> ret;
	ret = word_index.insert(std::pair<std::string, int>(word, id_generator));
	//if it does, update statistics	
	if(ret.second == false)
	{
		std::map<std::string,int>::iterator find_index = word_index.find(word);
		int id = find_index->second;
		std::map<int,int>::iterator find_cf = word_cf.find(id);
		find_cf->second += 1;
		//check if the word appeared in currently processed document
		bool result = helpers::contains(*added_in_doc, word);
		//if it didn't update document frequency and add the word to processed words in a document
		if(!result)
		{
			added_in_doc->push_back(word);
	                std::map<int,int>::iterator find_df = word_df.find(id);
	                find_df->second += 1;
		}
	}
	//if it doesn't, add a word to a wordmap with cf=1 and df=1
	else
	{
		reverse_index.insert(std::pair<int,std::string>(id_generator,word));
		word_cf.insert(std::pair<int,int>(id_generator,1));
		word_df.insert(std::pair<int,int>(id_generator,1));
		id_generator += 1;
	}
	return 0;
}
//adding a sequence of tokens to a wordmap, which is interpreted as document
int Wordmap::AddDocument(std::list<std::string> tokens)
{

	this->no_docs +=1;
	//empty list of words added in documents
	std::list<std::string> added_in_doc;
	//for each word in document
	for(auto token : tokens)
	{
		//add it to a wordmap
		AddWord(token, &added_in_doc);
	}
	return 0;
}

//trimming wordmap by min_count
void Wordmap::MinCount(int min_count)
{
	//save all word ids, which habe cf less then min_count
	std::list<int> toDelete;
	for(const auto& kv : word_index)
	{
		int id = kv.second;
		if(word_cf[id] < min_count)
			toDelete.push_back(id);
	}
	//delete entries related to saved ids
	for(int i : toDelete)
	{
		word_df.erase(i);
		word_cf.erase(i);
		std::string word = reverse_index[i];
		reverse_index.erase(i);
		word_index.erase(word);
	}
}	
