#include "errorHandler.h"
void handleFSError(int errorCode)
{
        switch (errorCode)
        {
		case 1:
			std::cout << "Operation not permitted" << std::endl;
                break;
		case 2:
                        std::cout << "No such file or directory" << std::endl;
		break;
		case 3:
		        std::cout << "No such process" << std::endl;
		break;
                default:
                        std::cout << "Unknown error code:" << errorCode <<std::endl;
        }
}
