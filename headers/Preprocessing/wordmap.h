#ifndef WORDMAP_H
#define WORDMAP_H

#include <map>
#include <list>
#include <string>
class Wordmap
{
private:
	std::map<std::string,int> word_index;
	std::map<int, std::string> reverse_index;
	std::map<int, int> word_cf;
	std::map<int, int> word_df;
	int id_generator;
	int no_docs;
	int no_toks;
public:
	Wordmap();
	void Join(Wordmap nmap);
	int AddWord(std::string word, std::list<std::string>* added_in_doc);
	int AddDocument(std::list<std::string> tokens);
	void SaveWordmap(std::string filename);
	void MinCount(int min_count);
	std::map<std::string,int>& GetWordIndex();
};

#endif // !WORDMAP_H