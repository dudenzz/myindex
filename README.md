UWAGA! Proszę o nie testowanie parsera - do odwołania. Muszę opracować sposób w którym będę sobie radził parsując "od lewej" z duplikatami nazw tagów - teraz generowane jest zbyt dużo plików. Poza tym wygenerowane pliki mogą stanowić wyzwanie dla systemu plików. 

Testować można natomiast indeks.

#Myindex project.

Make sure you have g++-8 installed. It should support C++17.

Make sure you have boost algorithms package for c++ installed.

Usage:

	1. Sync (or clone) with repository
	2. make
	3. ./bin/GeneralTest path_to_a_directory_with_any_set_of_documents number_of_processing_threads
	4. ./bin/ParserTest path_to_a_directory_with_pubmed_documents path_to_output_documents number_of_processing_threads

