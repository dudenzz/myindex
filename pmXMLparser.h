#include <string>

class PmXMLparser
{
	private:
		std::string init_path;
		std::string out_directory;
	public:
		PmXMLparser();
		PmXMLparser(std::string initial_path, std::string out_directory);
		void ParsePath(std::string dir, int no_processes);
};
