#include <iostream>
#include "cl_parser.h"
#include "config.h"
#include "hasher.h"

int main(int argc, char* argv[])
{
	int retVal = -1;
	try {
		CommandLineParser clp;
		if (auto config = clp.parseAndValidate(argc, argv)) {
			Hasher hasher;
			hasher.start(config);
			retVal = 0;
		}
	}
	catch (std::exception& e) {
		std::cerr << "[ERROR] " << e.what() << std::endl;
	}
	return retVal;
}
