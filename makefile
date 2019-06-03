CC=gcc
CXX=g++-8
CPPFLAGS=-std=c++17 -pthread 
FSFLAG=-lstdc++fs

SRCSP=parserTest.cpp pmXMLparser.cpp
SRCSM = test.cpp helpers.cpp index.cpp wordmap.cpp punktpp.cpp helpers.cpp errorHandler.cpp
parserTest: $(SRCSP)
	$(CXX) $(CPPFLAGS) -o ParserTest $(SRCSP) $(FSFLAG)
	$(CXX) $(CPPFLAGS) -o GeneralTest $(SRCSM) $(FSFLAG)



