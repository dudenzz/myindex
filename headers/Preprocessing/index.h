#ifndef INDEX_H
#define INDEX_H

#include <string>
#include <map>
#include <list>
#include <filesystem>
#include <iostream>
#include <unistd.h>
#include "wordmap.h"

//Index class is a current root class for the program
//it is supposed to initialize processes, 
//process the file names and directories,
//be able to store, save and load all the used data
class Index
{
private:
	//indicates whether documents are assigned for processing threads
	bool assignment;
	//number of processing threads
	int no_processes;
	//currently unused
	float** documents;
	//TODO size of a wordmap
	int wordmapSize;
	//root path of a file system directory with documents
	std::string root;
	//list of all file names
	std::list<std::string> filenames;
	//list of file names chopped, one sublist for each process
	std::list<std::string>* filenames_per_process;
	//a wordmap
	Wordmap wordmap;
	//erroneous state indicator, it equals 0 if the index is in normal state
	int error;
public:
	//a method which creates a wordmap for assigned set of documents
	void CreateWordmap();
	//a method which assigns the document file names for processing threads
	void AssignDocuments();
	//sets an erroneous state indicator
	void SetErrorneousState(int error);
	//TODO wordmap getter
	Wordmap GetWordmap();
	//TODO wordmap size getter
	int GetWordmapSize();
	//TODO wordmap size setter, currently not implemented
	void SetWordmapSize(int size);
	//simple constructor, assumes a single threaded execution
	Index(std::string docRootPath);
	//simple constructor, calculates the number of processing thread, tries to assign no_processes threads, if however document set is too small or  
	Index(std::string docRootPath, int no_processes);
	//TODO method which creates and trims the wordmap, gets rid of terms which do not accumulate min_count of occurences, sorts by idf and trims the wordmap to a size of "size" argument
	bool CreateWordmap(int minCount, int size);
	//TODO method which creates and trims the wordmap, gets rid of terms which do not accumulate min_count of occurences
	bool CreateWordmap(int minCount);
	//method, which trims the wordmap, TODO currently only mincount is implemented and it is hardcoded to 5
	void TrimWordmap();
};

#endif //!INDEX_H