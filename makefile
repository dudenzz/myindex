CC=gcc
CXX=g++-8
CPPFLAGS=-std=c++17 -pthread -Ofast
FSFLAG=-lstdc++fs

SRCSP= test/parserTest.cpp src/XMLParser/pmXMLparser.cpp
SRCSM= test/test.cpp src/helpers/helpers.cpp src/Preprocessing/index.cpp src/Preprocessing/wordmap.cpp src/Preprocessing/punktpp.cpp  src/helpers/errorHandler.cpp
SRCTTS= test/traintestsplitterTest.cpp headers/TrainTestSplitter/traintestsplitter.h src/TrainTestSplitter/traintestsplitter.cpp

SRCCLR= test/classifierTest.cpp \
	src/Classifier/liblinearclassifier.cpp \
	src/TrainTestSplitter/traintestsplitter.cpp \
	src/helpers/helpers.cpp src/Preprocessing/index.cpp \
	src/Preprocessing/wordmap.cpp \
	src/Preprocessing/punktpp.cpp \
	src/helpers/helpers.cpp \
	src/helpers/errorHandler.cpp 

SRCTOK= test/tokenizer_test.cpp src/Preprocessing/punktpp.cpp

parserTest: $(SRCSP)
	$(CXX) $(CPPFLAGS) -o bin/ParserTest.out $(SRCSP) $(FSFLAG)
	$(CXX) $(CPPFLAGS) -o bin/GeneralTest.out $(SRCSM) $(FSFLAG)
	$(CXX) $(CPPFLAGS) -o bin/SplitterTest.out $(SRCTTS)
	$(CXX) $(CPPFLAGS) -o bin/ClassifierTest.out $(SRCCLR) $(FSFLAG)
	$(CXX) $(CPPFLAGS) -o bin/TokenizerTest.out $(SRCTOK)

