#include "file_reader.h"
#include <filesystem>

FileReader::FileReader(const std::string& filePath, const size_t blockSize) :
	m_blockSize(blockSize),
	m_fileName(filePath),
	m_blocksDone(0)
{
	m_fileSize = std::filesystem::file_size(m_fileName);
	m_totalBlocks = (m_fileSize % m_blockSize == 0)
		? m_fileSize / m_blockSize
		: m_fileSize / m_blockSize + 1;

}

void FileReader::open()
{
	m_f.open(m_fileName, std::ios::binary | std::ios::in);
	if (!m_f.is_open()) {
		throw std::ios_base::failure("Cannot open file for read");
	}
	else {
		std::cout << "input file is open" << std::endl;
	}
}

size_t FileReader::fileSize()
{
	return m_fileSize;
}

size_t FileReader::blockSize()
{
	return m_blockSize;
}

size_t FileReader::getBlockNumAndInc() {
	return m_blocksDone.fetch_add(1);
}

bool FileReader::hasNext()
{
	return m_totalBlocks > m_blocksDone.load();
}

std::shared_ptr<ReadBlock> FileReader::readNext()
{
	auto result = std::make_shared<ReadBlock>();
	if (m_f.eof()) m_f.close();
	if (!m_f.is_open()) return result;

	const size_t count = blockSize();
	result->data.resize(count);
	m_f.read(result->data.data(), result->data.size());
	return result;
}
 