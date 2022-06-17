#pragma once

#include <filesystem>
#include <vector>
#include <thread>
#include <memory>
#include <mutex>
#include <boost/iostreams/device/mapped_file.hpp>
#include <boost/iostreams/code_converter.hpp>

typedef boost::iostreams::code_converter<boost::iostreams::mapped_file_source>  wmapped_file_source;
typedef boost::iostreams::code_converter<boost::iostreams::mapped_file_sink>    wmapped_file_sink;

struct Config;

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
	std::shared_ptr<char[]> read(size_t offset, size_t count);
	//std::filebuf* get() const;
	size_t fileSize() const;
	//bool isWholeFile() const;
	//bool hasNextMap(size_t pos) const;
	void nextMap(size_t pos);

	FileHandler(const FileHandler& ref) = delete;
	FileHandler& operator=(const FileHandler& ref) = delete;
	FileHandler(FileHandler&& ref) = delete;
	FileHandler& operator=(FileHandler&& ref) = delete;
private:
	const std::string m_filePath;
	size_t m_blockSize;
	size_t m_start;
	size_t m_fileSize;
	// size_t m_bufferSize;
	// const char* m_buffer;
	// std::unique_ptr<std::fstream> m_fs;
	std::mutex m_mtx;
	std::unique_ptr<boost::iostreams::mapped_file> m_file;
	boost::iostreams::mapped_file_base::mapmode m_mode;
};