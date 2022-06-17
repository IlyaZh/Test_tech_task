#pragma once

#include "file_handler.h"
#include <memory>

struct QueueWrapper {
    QueueWrapper(size_t offset_) : offset(offset_) {}
    size_t offset;
};

struct ThreadHandler {
    void operator()(std::atomic<size_t>& n, const size_t total, const char* buffer, size_t blockSize, std::shared_ptr<FileHandler> out);
    void handler(const size_t n, const char* buffer, size_t blockSize, std::shared_ptr<FileHandler> out);
};