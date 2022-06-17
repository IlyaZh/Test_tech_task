#include "cl_parser.h"
#include <string>
#include <iostream>
#include "config.h"

enum ArgumentsName {
    ExecutedFile,
    InputFile,
    OutputFile,
    BlockSize
};

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
                std::cerr << "[ERROR] Block size cannot be zero" << std::endl;
            }
            else {
                return settings; 
            }
        }
        catch (const std::bad_alloc& e) {
            std::cerr << "[ERROR] Cannot read data and allocate memory" << std::endl;
        } 
        catch (const std::invalid_argument& e) {
            std::cerr << "[ERROR] First argument conversion failed! " << std::endl;
        }
        catch (const std::out_of_range& e) {
            std::cerr << "[ERROR] First argument value out of range" << std::endl;
        }
    }
    else {
        std::cerr << "Wrong amount of arguments." << std::endl;
        std::cerr << "Please enter 3 arguments <input_file.txt> <output_file.txt> <block_size in bytes>" << std::endl;
    }
    return std::shared_ptr<Config>();
}
