#include <string>
#include <map>
#include <list>
#include <filesystem>
#include <iostream>
#include <unistd.h>
#include "wordmap.h"
class Index
{
private:
	bool assignment;
	int no_processes;
	float** documents;
	int wordmapSize;
	std::string root;
	std::list<std::string> filenames;
	std::list<std::string>* filenames_per_process;
	Wordmap wordmap;
	int error;
public:
	void CreateWordmap();
	void AssignDocuments();
	void SetErrorneousState(int error);
	Wordmap GetWordmap();
	int GetWordmapSize();
	void SetWordmapSize(int size);
	Index(std::string docRootPath);
	Index(std::string docRootPath, int no_processes);
	bool CreateWordmap(int minCount, int size);
	bool CreateWordmap(int minCount);
	void TrimWordmap();
};
