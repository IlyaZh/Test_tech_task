#pragma once

#include <memory>
#include <vector>
#include <thread>
#include <utility>
#include "thread_handler.h"

struct Config;
class FileHandler;

class Hasher {
public:
    void start(std::shared_ptr<Config> config);

private:
    static const size_t HashSize {1};
};