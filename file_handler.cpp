#include "file_handler.h"
#include <iostream>
#include <fstream>
#include "config.h"
#include <mutex>
#include <algorithm>
#include <boost/iostreams/device/mapped_file.hpp>

using namespace boost::iostreams;

FileHandler::FileHandler(const std::string& filePath, size_t blockSize, OpenMode mode) :
	m_filePath(filePath),
	m_blockSize(blockSize),
	m_fileSize(0),
	m_file(std::make_unique<mapped_file>())
{
	// Сохраняем режим открытия файла (Только чтение\ чтение и запись)
	m_mode = (mode == OpenMode::Read) 
				?  mapped_file_base::mapmode::readonly 
				: mapped_file_base::mapmode::readwrite;

	if(mode == OpenMode::ReadWrite) {
		// Создаем файл для записи, если он не существует
		// Поток сам закрывает файл в деструкторе
		std::fstream f(m_filePath, std::ios::binary | std::ios::out);
	}
	// Заодно посчитаем размер файла
	m_fileSize = std::filesystem::file_size(m_filePath);
	m_totalBlocks = (m_fileSize % m_blockSize == 0)
		? m_fileSize / m_blockSize
		: m_fileSize / m_blockSize + 1;
}

FileHandler::~FileHandler() {
	if(m_file->is_open())
		m_file->close();
}

void FileHandler::open(size_t length) {
	auto newLength = mapped_file::max_length;

	switch(m_mode) {
		case mapped_file_base::mapmode::readonly:
			try {
				m_file->open(
					m_filePath,
					m_mode,
					mapped_file::max_length,
					0
				);
			} catch(std::exception&e) {
				throw;
			}
		break;

		case mapped_file_base::mapmode::readwrite:
			// Для записи открываем файл и резервируем место в нём под заданное число байт
			mapped_file_params params(m_filePath);
			params.flags = m_mode;
			params.new_file_size = length;
			params.length = length;
			m_file->open(params);
		break;
	}
	if(!m_file->is_open()) {
		throw std::invalid_argument("Can't open file " + m_filePath);
	}}

const char* FileHandler::data() const {
	return m_file->const_data();
}

size_t FileHandler::size() const {
	return m_file->size();
}

void FileHandler::write(char value, size_t pos) {
	if(m_mode == mapped_file_base::mapmode::readwrite) {
		if(pos >= m_file->size()) {
			throw std::out_of_range("File write position is out of range ");
		}
		std::unique_lock lk(m_mtx);
		m_file->data()[pos] = value;
	}
}
size_t FileHandler::fileSize() const {
	return m_fileSize;
}

size_t FileHandler::blockSize()
{
	return m_blockSize;
}

bool FileHandler::hasNext()
{
	return m_totalBlocks > m_blocksDone.load();
}

std::shared_ptr<FileBlock> FileHandler::readNext()
{
	auto result = std::make_shared<FileBlock>();
	const size_t n = m_blocksDone.fetch_add(1);
	if (!m_file->is_open() && n < m_totalBlocks) return result;
	result->n = n;
	const size_t offset = blockSize() * result->n;
	result->data = &m_file->const_data()[offset];
	result->len = (m_file->size()-offset >= blockSize()) ? blockSize() : m_file->size() - offset;
	return result;
}
