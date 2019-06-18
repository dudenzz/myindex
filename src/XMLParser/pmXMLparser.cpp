#include "../../headers/XMLParser/pmXMLparser.h"
#include "../../headers/helpers/helpers.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <chrono>
#include <filesystem>
#include <future>
#include <list>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp>
namespace fs = std::filesystem;


//THIS FILE IS CURRENTLY WORK IN PROGRESS, BASIC VERSION WILL CONTAIN PROPER COMMENTS
PmXMLparser::PmXMLparser(std::string initial_path, std::string out_directory)
{
	//TODO
}

PmXMLparser::PmXMLparser()
{
	std::string file = helpers::get_file_string("data/key_tags");
	boost::split(this->key_tokens, file, [](char c){return c == '\n';});
	this->key_tokens.pop_back();
}



int Parse(std::list<std::string> filepaths, std::string out_path, std::atomic<int>* progress, std::vector<std::string> key_tokens)
{
	int iter = 0;
	if(filepaths.size() == 0)
		progress->store(100);
	for(auto filepath : filepaths)
	{
		progress->store((iter*100)/filepaths.size());
		std::ifstream fileIn;
		fileIn.open(filepath);
		std::string line;
		int no_a = 0;
		std::string pmid;
		std::string text;
		int ff = 1;
		int add_this_line = 0;
		while (std::getline(fileIn, line))
		{
			int line_added = 0;
			trim(line);
			for(auto token: key_tokens)
			{
				if(token.compare("PubmedArticle") == 0)
				{
					if(line.compare("<" + token + ">") == 0)
					{
						if(pmid.compare(""))
						{
							text += "</document>";
							std::ofstream oFile;
							oFile.open(out_path + pmid + ".xml");
							oFile << text;
							oFile.close();
						}
						else
						{
							if(ff)
								ff = 0;
							else
								std::cout << "warning: a file without ID" << std::endl;
						}
						pmid = "";
						text = "<document>";
					}
				}
				else if(token.compare("PMID") == 0)
				{

	                                if(boost::starts_with(line, "<PMID"))
                                        {
						int start = line.find(">");
						int stop = line.find("</");
						std::string pmid_g = line.substr(start+1, stop-start-1);
						pmid = pmid_g;
					}
				}
				else
				{
					int final_line = 0;
					if(boost::starts_with(line, "<" + token + " ") || boost::starts_with(line, "<" + token + ">"))
					{
						text += "\n";
						text += "<" + token + ">\n";
						int tag_pos = line.find(">");
						int line_ln = line.length();
						add_this_line = 1;
						line = line.substr(tag_pos+1);
					}
					if(boost::ends_with(line,"</" + token +">"))
					{
						final_line = 1;
						int text_l = line.length() - token.length() - 3;
						line = line.substr(0,text_l);
					}
                                        if(boost::starts_with(line,"</" + token +">"))
                                        {
						line = "";
					}

					if(add_this_line && !line_added)
					{
						line_added = 1;
						text += line;
						text += "\n";
					}
					if(final_line)
					{
						text+= "</" + token + ">";
						add_this_line = 0;
					}
				}
			}
		}
        	auto end = std::chrono::high_resolution_clock::now();
		iter += 1;
		progress->store((iter*100)/filepaths.size());
	}
	return 0;
}
void PmXMLparser::ParsePath(std::string dir, std::string opath, int no_processes)
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
		threads[i] = std::async(std::launch::async,Parse,paths[i],opath,&atomics[i],this->key_tokens);
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

