#include <fstream>
#include <algorithm> 
#include <cctype>
#include <locale>
#include <string>
#include <list>

static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}

// trim from start (copying)
static inline std::string ltrim_copy(std::string s) {
    ltrim(s);
    return s;
}

// trim from end (copying)
static inline std::string rtrim_copy(std::string s) {
    rtrim(s);
    return s;
}

// trim from both ends (copying)
static inline std::string trim_copy(std::string s) {
    trim(s);
    return s;
}

namespace helpers{
	template<typename T>
	bool contains(std::list<T> & listOfElements, const T & element)
	{
        	// Find the iterator if element in list
        	auto it = std::find(listOfElements.begin(), listOfElements.end(), element);
        	//return if iterator points to end or not. It points to end then it means element
        	// does not exists in list
        	return it != listOfElements.end();
	}
	static inline std::string get_file_string(std::string path)
	{
    		std::ifstream ifs(path);
    		return std::string((std::istreambuf_iterator<char>(ifs)),
                 	   (std::istreambuf_iterator<char>()));
	}
}
