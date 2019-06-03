#include "pmXMLparser.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <chrono>
#include <filesystem>
#include <future>
#include <list>
namespace fs = std::filesystem;

PmXMLparser::PmXMLparser(std::string initial_path, std::string out_directory)
{
	//TODO
}

PmXMLparser::PmXMLparser()
{
	//TODO
}


int Parse(std::list<std::string> filepaths, std::atomic<int>* progress)
{
	int iter = 0;
	for(auto filepath : filepaths)
	{
		progress->store((iter*100)/filepaths.size());
		std::string initial_token = "<PubmedArticle>";
		std::string end_token = "</PubmedArticle>";
		std::ifstream fileIn;
		fileIn.open(filepath);
		std::string line;
		int no_a = 0;
		while (std::getline(fileIn, line))
		{
			while(line[0] == ' ' || line[0] == '\t')
				line.erase(0,1);
			while(line[line.size()-1] == ' ' || line[line.size() - 1] == '\t')
				line.erase(line.size() - 2);
			if(line.compare(initial_token) == 0)
			{
			//std::cout << line << std::endl;
				no_a++;
			}
			//if(line.compare(end_token) == 0)
			//	std::cout << line << std::endl;
		}
        	auto end = std::chrono::high_resolution_clock::now();
		iter += 1;
	}
	return 0;
}
void PmXMLparser::ParsePath(std::string dir, int no_processes)
{
	int iter = 0;
	std::list<std::string> paths[no_processes];
	std::future<int> threads[no_processes];
	std::atomic<int> atomics[no_processes];
        for(auto& p: fs::recursive_directory_iterator(fs::path(dir)))
                  if(!fs::is_directory(p.path()))
		  {
			paths[iter].push_back(p.path());
			iter += 1;
			iter %= no_processes;
		  }
	for(int i = 0; i<no_processes; i++)
	{
		threads[i] = std::async(std::launch::async,Parse,paths[i],&atomics[i]);
	}
        bool check = 2;
        while(check)
        {
                std::string prtprg = "";
                check = 0;
                for(int i = 0; i<no_processes; i++)
                {
                        int progress_val = atomics[i].load();
                        if(progress_val < 100)
                        {
                                check = 1;
                        }
                        prtprg += "Thread " + std::to_string(i) + " : " + std::to_string(progress_val) + "% ";
                }
                //std::this_thread::sleep_for (std::chrono::seconds(1));
                prtprg = prtprg;
                //std::cout << prtprg;
                printf("%c[2K",0x1B);
                std::cout << "\r" <<prtprg << std::flush;
                std::this_thread::sleep_for (std::chrono::seconds(1));
        }
	for(int i = 0; i<no_processes; i++)
	{
		threads[i].wait();
	}
}

