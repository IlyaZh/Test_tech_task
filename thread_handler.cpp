#include <iostream>
#include "thread_handler.h"
#include "file_handler.h"
#include "crc8.h"
#include <atomic>

void ThreadHandler::operator()(std::atomic<size_t>& n, const size_t total, const char* buffer, size_t blockSize, std::shared_ptr<FileHandler> out)
{

    while(auto pos = n.fetch_add(1)) {
        if (--pos >= total) break;
        const size_t offset = blockSize*pos;
        handler(pos, &buffer[offset], blockSize, out);
    }
}

void ThreadHandler::handler(const size_t n, const char* buffer, size_t blockSize, std::shared_ptr<FileHandler> out)
{
    auto res = crc8(buffer, blockSize);
    out->write(res, n);
}
