#pragma once 

#include <iostream>
#include <fstream>
#include <memory>
#include <atomic>
#include <vector>
#include "config.h"

struct ReadBlock {
	size_t idx{0};
	std::vector<char> data;
};

class FileReader {
	const size_t m_blockSize;
	const std::string m_fileName;
	size_t m_fileSize;
	size_t m_totalBlocks;
	std::atomic<size_t> m_blocksDone;
	std::fstream m_f;
	
public:
	FileReader(const std::string& filePath, const size_t blockSize);
	void open();
	size_t fileSize();
	size_t blockSize();
	size_t getBlockNumAndInc();
	bool hasNext();
	std::shared_ptr<ReadBlock> readNext();
};