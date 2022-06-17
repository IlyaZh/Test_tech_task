#include <iostream>
#include "cl_parser.h"
#include "config.h"
#include "hasher.h"

int main(int argc, char* argv[])
{
	CommandLineParser clp;
	if (auto config = clp.parseAndValidate(argc, argv)) {
		try {
			Hasher hasher;
			hasher.start(config);
		} catch(std::exception& e) {
			std::cerr << "[ERROR] " << e.what() << std::endl;
		}
		return 0;
	}
	return -1;
}
