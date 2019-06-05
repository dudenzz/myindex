#include "../../headers/Preprocessing/index.h"
#include "../../headers/helpers/errorHandler.h"
#include "../../headers/Preprocessing/punktpp.h"
#include "../../headers/helpers/helpers.h"
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
//TODO
int Index::GetWordmapSize()
{
	return wordmapSize;
}
//this returns a copy of a wordmap, use with care
Wordmap Index::GetWordmap()
{
	return wordmap;
}
//TODO
void Index::SetWordmapSize(int size)
{
	wordmapSize = size;
}
//TODO
bool Index::CreateWordmap(int minCount, int size)
{
	SetWordmapSize(size);
	return CreateWordmap(minCount);
}
//TODO
bool Index::CreateWordmap(int minCount)
{
	return true;
}
//simple constructor
Index::Index(std::string docRootPath)
{
	Index(docRootPath, 1);
}
//base constructor
Index::Index(std::string docRootPath, int no_processes)
{
	const fs::path path(docRootPath); // Constructing the path from a string is possible.
    	std::error_code ec; // For using the non-throwing overloads of functions below.
	//if given path is a directory
    	if (fs::is_directory(path, ec))
    	{
        	root = docRootPath;
		std::cout << "Succesfully assigned a document root path:\n\t" << root << std::endl;
		//recursively iterate over file names in the directory
		for(auto& p: fs::recursive_directory_iterator(root))
        		if(!fs::is_directory(p.path()))
				//and append them into the filenames list
				filenames.push_back(p.path());
		//get the number of documents to be processed
		int fnsize = filenames.size();
		std::cout << "There are " << fnsize << " files on the list" << std::endl;
		//if there is less than 1000 documents per processing thread
		if (fnsize / 1000 < no_processes-1)
		{
			std::cout << "Planned number of processes is too large" << std::endl << "Setting number of processes to "<<fnsize/1000 + 1 <<std::endl;
			//set the number of processing threads so each has to process at least 1000 documents
			this->no_processes = fnsize / 1000 + 1;
		}
		else
		{
			std::cout << "Setting number of processes to " << no_processes << std::endl;
			//otherwise assume the number of processing threads given in the argument
			this->no_processes = no_processes;
		}
		//get the number of CPUs
		int numCPU = sysconf(_SC_NPROCESSORS_ONLN);
		std::cout << "Avaliable number of cores: " << numCPU << std::endl;
		//if there are more processing threads than number of CPUs
		if(numCPU < this->no_processes)
		{
			std::cout<<"Changing number of processes to fit number of cores" << std::endl;
			//change the number of processing threads to the number of cpus
			this->no_processes = numCPU;
		}
		//indicate that file name list has to be split among processes
		this -> assignment = false;
    	}
	else
	{
	   //handle filesystem error
	   handleFSError(ec.value());
	}
	//TODO
	wordmapSize = 100000;
}

void Index::AssignDocuments()
{
	//create an array of lists of filenames, one list for each processing thread
	filenames_per_process = new std::list<std::string>[no_processes];
	int size = this->filenames.size();
	//calculate size of a single list
	int size_per_process = size/this->no_processes + 1;
	int current_process = 0;
	int current_document = 0;
	//split the filenames into created lists
	for(std::string fn : filenames)
	{
		filenames_per_process[current_process].push_back(fn);
		current_document += 1;
		if (current_document % size_per_process == 0)
			current_process += 1;
	}
	std::cout << "Documents assigned for each process" << std::endl;
	//indicate that lists for each processing thread have been created
	this->assignment = true;
}

//a method for a single thread wordmap creating
Wordmap CreateSubWordmap(int tid, std::list<std::string> filenames, std::atomic<int>* progress)
{
	Wordmap wm = Wordmap();
	//get the number of docs to be processed
	int size = filenames.size();
	int i = 0;
	//for each file in file in the map
	for(std::string file : filenames)
	{
		//update the iterator
		i += 1;
		//every 1000 documents
		if(i % 1000 == 0);
		{
			//update the progress
			progress->store ((i*100)/size);
		}
		//tokenize the file
		auto var = helpers::get_file_string(file);
		PunktTokenizer tok = PunktTokenizer();
		auto tokens = tok._word_tokenize_simple(var);
		//add tokens to the wordmap
		wm.AddDocument(tokens);
	}
	//return the wordmap
	return wm;
}


void Index::CreateWordmap()
{
	//measure the time
	auto begin = std::chrono::high_resolution_clock::now();
	//if documents are not assigned for processing thread	
	if(!this->assignment)
	{	
		//exit
		std::cout << "You have to assign documents to processes first";
		return;
	}
	
	std::cout << this->filenames_per_process[0].front() << std::endl;
	//create arrays for threads and progress bars	
	std::future<Wordmap> threads[this->no_processes];
	std::atomic<int> atomics[this->no_processes];
	//create an empty wordmap
	Wordmap wm;
	//create and launch the processing threads
        for(long unsigned int i = 0; i < this->no_processes; i++ ) {
      		std::cout << "wordmap creator : creating thread, " << i << std::endl;
		std::list<std::string> fns = this->filenames_per_process[i];
   		threads[i] = std::async(std::launch::async,CreateSubWordmap,i,fns,&atomics[i]);
	}
	bool check = 2;
	//while any of the processing threads is at below 100% progress	
	while(check)
	{
		std::string prtprg = "";
		//assume that all processes are finished
		check = 0;
		//for each procress
		for(int i = 0; i<this->no_processes; i++)
		{
			int progress_val = atomics[i].load();
			//if a process progress is at below 100, it is not finished yet
			if(progress_val < 100)
			{
				check = 1;
			}
			//save the progress in the log line
			prtprg += "Thread " + std::to_string(i) + " : " + std::to_string(progress_val) + "% ";
		}
		prtprg = prtprg;
		//flush the line		
		printf("%c[2K",0x1B);
		//print progress
                std::cout << "\r" <<prtprg << std::flush;
		//wait one second		
		std::this_thread::sleep_for (std::chrono::seconds(1));
        }
	//join the created wordmaps
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
	//store the calculated wordmap as a current one
	this->wordmap = wm;
	auto end = std::chrono::high_resolution_clock::now();
	std::cout<< "Elapsed: " << std::chrono::duration_cast<std::chrono::seconds>(end-begin).count() << std::endl; 
}
//TODO - wordmap trimming method
void Index::TrimWordmap()
{
	wordmap.MinCount(5);
}
