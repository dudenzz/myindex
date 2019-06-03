#include "pmXMLparser.h"
int main(int argc, char **argv)
{
	PmXMLparser parser = PmXMLparser();
	parser.ParsePath(argv[1], atoi(argv[2]));
}
