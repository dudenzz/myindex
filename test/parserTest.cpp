#include "../headers/XMLParser/pmXMLparser.h"
int main(int argc, char **argv)
{
	PmXMLparser parser = PmXMLparser();
	parser.ParsePath(argv[1], argv[2], atoi(argv[3]));
}
