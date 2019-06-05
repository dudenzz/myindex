CC=gcc
CXX=g++-8
CPPFLAGS=-std=c++17 -pthread 
FSFLAG=-lstdc++fs

SRCSP= test/parserTest.cpp src/XMLParser/pmXMLparser.cpp
SRCSM= test/test.cpp src/helpers/helpers.cpp src/Preprocessing/index.cpp src/Preprocessing/wordmap.cpp src/Preprocessing/punktpp.cpp  src/helpers/errorHandler.cpp
parserTest: $(SRCSP)
	$(CXX) $(CPPFLAGS) -o bin/ParserTest.out $(SRCSP) $(FSFLAG)
	$(CXX) $(CPPFLAGS) -o bin/GeneralTest.out $(SRCSM) $(FSFLAG)



