#pragma once

#include <string>

struct Config {
    Config(const std::string& in, const std::string& out, const size_t size) :
        inputFilePath(in),
        outputFilePath(out),
        blockSize(size)
    {}
    std::string inputFilePath;
    std::string outputFilePath;
    size_t blockSize{ 0 };
};
