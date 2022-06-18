#pragma once

#include <filesystem>
#include <vector>
#include <thread>
#include <memory>
#include <mutex>
#include <boost/iostreams/device/mapped_file.hpp>
#include <boost/iostreams/code_converter.hpp>
#include "file_reader.h"

typedef boost::iostreams::code_converter<boost::iostreams::mapped_file_source>  wmapped_file_source;
typedef boost::iostreams::code_converter<boost::iostreams::mapped_file_sink>    wmapped_file_sink;

struct Config;

struct FileBlock {
	const char* data{nullptr};
	size_t len{0};
	size_t n{0};
};

class FileHandler {
public:
	enum class OpenMode {
		Read = 1,
		ReadWrite = 3
	};

	FileHandler(const std::string& filePath, size_t blockSize, OpenMode mode = OpenMode::Read);
	~FileHandler();

	const char* data() const;
	size_t size() const;
	void write(char value, size_t pos);

	void open(size_t length = 0);
	size_t fileSize() const;
	size_t blockSize();
	bool hasNext();
	std::shared_ptr<FileBlock> readNext();

	FileHandler(const FileHandler& ref) = delete;
	FileHandler& operator=(const FileHandler& ref) = delete;
	FileHandler(FileHandler&& ref) = delete;
	FileHandler& operator=(FileHandler&& ref) = delete;
private:
	const std::string m_filePath;
	size_t m_blockSize;
	size_t m_fileSize;
	size_t m_totalBlocks;
	std::atomic<size_t> m_blocksDone;
	std::mutex m_mtx;
	std::unique_ptr<boost::iostreams::mapped_file> m_file;
	boost::iostreams::mapped_file_base::mapmode m_mode;
};