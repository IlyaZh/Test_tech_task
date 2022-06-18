#include <iostream>
#include <atomic>
#include "hasher.h"
#include "config.h"
#include <fstream>
#include "file_handler.h"
#include <thread>
#include "thread_handler.h"

void Hasher::start(std::shared_ptr<Config> config) {
    // map файла в память на чтение
    auto inFile = std::make_shared<FileHandler>(config->inputFilePath, config->blockSize);
    try {
        inFile->open();
    } catch(const std::exception& e) {
        throw;
    }

    // Размер файла для чтения
    const size_t sizeIn = std::filesystem::file_size(config->inputFilePath); 
    // Количество блоков для покрытия всего файла
    const size_t totalBlocks = ((inFile->fileSize() % config->blockSize) == 0)
        ? inFile->fileSize() / config->blockSize
        : inFile->fileSize() / config->blockSize + 1;
    // Размер выходного файла = размер блока * количество блоков.
    const size_t sizeOutFile = HashSize * totalBlocks;

    // map файла в память на запись
    auto outFile = std::make_shared<FileHandler>(config->outputFilePath, config->blockSize, FileHandler::OpenMode::ReadWrite);
    try {
        outFile->open(sizeOutFile);
    } catch(const std::exception& e) {
        throw;
    }
    
    // максимальное количество аппаратных потоков или выбираем 1
    const size_t threadsCount = std::max(1u, std::thread::hardware_concurrency());

    std::vector<std::thread> threads;
    // Функтор, исполняемый в потоке
    ThreadHandler threadHandler;
    // Количество целых блоков (если размер файла нацело делится на размер блока)
    const size_t wholeBlocks = sizeIn / config->blockSize;
    for(size_t i = 0; i < threadsCount - 1; ++i) {
        threads.emplace_back(threadHandler, inFile, outFile);
    }
    threadHandler(inFile, outFile);
    
    for(auto& t : threads) {
        if(t.joinable())
            t.join();
    }
}
