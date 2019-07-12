Można testować indeks i parser. 

#Myindex project.

Make sure you have g++-8 installed. It should support C++17.

Make sure you have boost algorithms package for c++ installed.

Usage:

	1. Sync (or clone) with repository
	2. make
	3. ./bin/GeneralTest path_to_a_directory_with_any_set_of_documents number_of_processing_threads
	4. ./bin/ParserTest path_to_a_directory_with_pubmed_documents path_to_output_documents number_of_processing_threads
	5. ./bin/CLassifierTest.out input_parsedDocsRootDir output_trainTestDocsRoot processesNumber
