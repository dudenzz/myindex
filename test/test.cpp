#include "../headers/Preprocessing/index.h"
#include <string>

int main(int argc, char* argv[])
{
	if(argc != 3)
	{
		std::cout << "wrong number of parameters" << std::endl;
		return 0;
	}
	Index c = Index(argv[1], atoi(argv[2]));
	c.AssignDocuments();
	c.CreateWordmap();
	c.TrimWordmap();
	//c.GetWordmap().SaveWordmap("20news_wm");
}
