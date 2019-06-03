#include "index.h"
#include "errorHandler.h"
#include "punktpp.h"
#include "helpers.h"
#include <thread>
#include <fstream>
#include <list>
#include <atomic>
#include <ctime>
#include <chrono>
#include <future>
namespace fs = std::filesystem;

void Index::SetErrorneousState(int error)
{
	/*
		0 - no errors
		1 - non critical error
			-index does not have to be remade
			-current process gets stopped and any changes are supposed to be reverted
			-once the changes get reverted errorState = 0
		2 - critical error
			-index has to be remade
			-changes are irrecoverable
	*/
	this->error = error;
}
int Index::GetWordmapSize()
{
	return wordmapSize;
}
Wordmap Index::GetWordmap()
{
	return wordmap;
}
void Index::SetWordmapSize(int size)
{
	wordmapSize = size;
}

bool Index::CreateWordmap(int minCount, int size)
{
	SetWordmapSize(size);
	return CreateWordmap(minCount);
}

bool Index::CreateWordmap(int minCount)
{
	return true;
}

Index::Index(std::string docRootPath)
{
	Index(docRootPath, 1);
}
Index::Index(std::string docRootPath, int no_processes)
{
	const fs::path path(docRootPath); // Constructing the path from a string is possible.
    	std::error_code ec; // For using the non-throwing overloads of functions below.
    	if (fs::is_directory(path, ec))
    	{
        	root = docRootPath;
		std::cout << "Succesfully assigned a document root path:\n\t" << root << std::endl;
		for(auto& p: fs::recursive_directory_iterator(root))
        		if(!fs::is_directory(p.path()))
				filenames.push_back(p.path());
		int fnsize = filenames.size();
		std::cout << "There are " << fnsize << " files on the list" << std::endl;
		if (fnsize / 1000 < no_processes-1)
		{
			std::cout << "Planned number of processes is too large" << std::endl << "Setting number of processes to "<<fnsize/1000 + 1 <<std::endl;
			this->no_processes = fnsize / 1000 + 1;
		}
		else
		{
			std::cout << "Setting number of processes to " << no_processes << std::endl;
			this->no_processes = no_processes;
		}
		int numCPU = sysconf(_SC_NPROCESSORS_ONLN);
		std::cout << "Avaliable number of cores: " << numCPU << std::endl;
		if(numCPU < this->no_processes)
		{
			std::cout<<"Changing number of processes to fit number of cores" << std::endl;
			this->no_processes = numCPU;
		}
		this -> assignment = false;
    	}
	else
	{
	   handleFSError(ec.value());
	}
	wordmapSize = 100000;
}

void Index::AssignDocuments()
{
	filenames_per_process = new std::list<std::string>[no_processes];
	int size = this->filenames.size();
	int size_per_process = size/this->no_processes + 1;
	int current_process = 0;
	int current_document = 0;
	for(std::string fn : filenames)
	{
		filenames_per_process[current_process].push_back(fn);
		current_document += 1;
		if (current_document % size_per_process == 0)
			current_process += 1;
	}
	std::cout << "Documents assigned for each process" << std::endl;
	this->assignment = true;
}


Wordmap CreateSubWordmap(int tid, std::list<std::string> filenames, std::atomic<int>* progress)
{
	Wordmap wm = Wordmap();

	int size = filenames.size();
	int i = 0;
	//std::ofstream fileOut;
	//fileOut.open("Thread"+std::to_string(tid));
	//std::cout << "Thread is processing " << i << " files." << std::endl;
	for(std::string file : filenames)
	{
		i += 1;
		if(i % 1000 == 0);
		{
			//std::cout << (i/(float)size)*100 << "% done" << std::endl;
			progress->store ((i*100)/size);
		}
		auto var = helpers::get_file_string(file);
		PunktTokenizer tok = PunktTokenizer();
		auto tokens = tok._word_tokenize_simple(var);
		wm.AddDocument(tokens);
	}
	//fileOut.close();
	return wm;
}


void Index::CreateWordmap()
{
	auto begin = std::chrono::high_resolution_clock::now();
	if(!this->assignment)
	{
		std::cout << "You have to assign documents to processes first";
		return;
	}
	std::cout << this->filenames_per_process[0].front() << std::endl;
	std::future<Wordmap> threads[this->no_processes];
	std::atomic<int> atomics[this->no_processes];
	Wordmap wm;
        for(long unsigned int i = 0; i < this->no_processes; i++ ) {
      		std::cout << "wordmap creator : creating thread, " << i << std::endl;
		std::list<std::string> fns = this->filenames_per_process[i];
   		threads[i] = std::async(std::launch::async,CreateSubWordmap,i,fns,&atomics[i]);
	}
	bool check = 2;
	while(check)
	{
		std::string prtprg = "";
		check = 0;
		for(int i = 0; i<this->no_processes; i++)
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
	std::cout<<std::endl<<"Wordmap created joining the threads"<<std::endl;
	for(long unsigned int i = 0; i < this->no_processes; i++ ) {
		threads[i].wait();
		if(i == 0)
		{
			wm = threads[i].get();
		}
		else
		{
			wm.Join(threads[i].get());
		}
	}
	this->wordmap = wm;
	auto end = std::chrono::high_resolution_clock::now();
	std::cout<< "Elapsed: " << std::chrono::duration_cast<std::chrono::seconds>(end-begin).count() << std::endl; 
}

void Index::TrimWordmap()
{
	wordmap.MinCount(5);
}
