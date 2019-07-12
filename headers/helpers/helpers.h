#ifndef HELPERS_H
#define HELPERS_H

#include <fstream>
#include <algorithm> 
#include <cctype>
#include <locale>
#include <string>
#include <list>
#include <boost/algorithm/string.hpp>
#include <iostream>

/*
helpers file contains definitions of procedures and functions which are used across the project
*/



//ltrim trims the left part of the string, it works on the string directly
static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));
}

//rtrim trims the right part of the string, it works on the string directly
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

//trim trims the string, it works on the string directly
static inline void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}

//ltrim_copy trims the left part of the string, it works on a copy of a string and returns a result
static inline std::string ltrim_copy(std::string s) {
    ltrim(s);
    return s;
}

//rtrim_copy trims the right part of the string, it works on a copy of a string and returns a result
static inline std::string rtrim_copy(std::string s) {
    rtrim(s);
    return s;
}

//rtrim_copy trims the string, it works on a copy of a string and returns a result
static inline std::string trim_copy(std::string s) {
    trim(s);
    return s;
}

namespace helpers{
	//contains looks for an element on a list, it returns true if a list constains a given element and false if the list doesn't contain the element
	template<typename T>
	bool contains(std::list<T> & listOfElements, const T & element)
	{
        	// Find the iterator if element in list
        	auto it = std::find(listOfElements.begin(), listOfElements.end(), element);
        	//return if iterator points to end or not. It points to end then it means element
        	// does not exists in list
        	return it != listOfElements.end();
	}
	//get_file_string returns the contents of a file given in path argument
	static inline std::string get_file_string(std::string path)
	{
    		std::ifstream ifs(path);
			if(!ifs) std::cout << "Helpers | Error: cannot open given path: " << path << std::endl;
    		return std::string((std::istreambuf_iterator<char>(ifs)),
                 	   (std::istreambuf_iterator<char>()));
	}
	static inline std::vector<std::string> read_collocations_tokens()
	{
		auto text = get_file_string("data/col_tokens");
		std::vector<std::string> tokens;
		boost::split(tokens,text, [](char c){return c == ' ';});
		return tokens;
	}
}

#endif // !HELPERS_H
