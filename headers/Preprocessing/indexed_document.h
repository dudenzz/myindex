#include <list>
#include <map>
#include <string>
class IndexedDocument
{
	private:
		std::map<int,std::list<int>> values;
	public:
		IndexedDocument();
		std::string to_string();
		void add_token(int index, int position);
};
