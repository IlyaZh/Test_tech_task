#include "cl_parser.h"
#include <string>
#include <iostream>
#include "config.h"

std::shared_ptr<Config> CommandLineParser::parseAndValidate(int count, char* args[]) 
{
    if (count == 4) {
        try {
            auto settings = std::make_shared<Config>(
                std::string(args[1]),
                std::string(args[2]),
                std::stoull(std::string(args[3]))
                );
            if (settings->blockSize == 0) {
                throw std::invalid_argument("Block size cannot be zero");
            }
            else {
                return settings; 
            }
        }
        catch (const std::exception& e) {
            throw;
        }
    }
    else {
        std::cout << "Wrong amount of arguments." << std::endl;
        std::cout << "Please enter 3 arguments <input_file.txt> <output_file.txt> <block_size in bytes>" << std::endl;
    }
    return std::shared_ptr<Config>();
}
