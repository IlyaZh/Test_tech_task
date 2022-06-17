#pragma once 

#include <iostream>
#include <fstream>
#include <filesystem>
#include <memory>
#include <atomic>
#include "config.h"

class FileReader {
	const size_t m_blockSize;
	const std::string m_fileName;
	const size_t m_fileSize;
	const size_t m_totalBlocks;
	std::atomic<size_t> m_blocksDone(0);
	
public:
	FileReader(std::shared_ptr<Config> config, const size_t blockSize);
	size_t fileSize();
	size_t blockNumAndInc();
	bool hasNext();
	void readNext();
};