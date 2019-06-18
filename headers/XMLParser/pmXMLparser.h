#include <string>
#include <vector>
//the basis of XML parser
class PmXMLparser
{
	private:
		std::string init_path;
		std::string out_directory;
		std::vector<std::string> key_tokens;
	public:
		PmXMLparser();
		PmXMLparser(std::string initial_path, std::string out_directory);
		void ParsePath(std::string dir, std::string out_path, int no_processes);
};
