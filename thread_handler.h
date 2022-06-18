#pragma once

#include "file_handler.h"
#include "file_reader.h"
#include <memory>

struct ThreadHandler {
    void operator()(std::shared_ptr<FileHandler> reader, std::shared_ptr<FileHandler> out);
private:
    void handler(const size_t n, const char* buffer, size_t size, std::shared_ptr<FileHandler> out);
    char crc8(const char* pcBlock, size_t len);
    static const unsigned char Crc8Table[256];
};