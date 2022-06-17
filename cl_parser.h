#pragma once

#include <memory>
#include <string>


struct Config;

class CommandLineParser {
public:
	std::shared_ptr<Config> parseAndValidate(int count, char* args[]);
};


