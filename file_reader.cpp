#include "file_reader.h"

FileReader::FileReader(std::shared_ptr<Config> config, const size_t blockSize) :
	m_blockSize(blockSize)
{
}
