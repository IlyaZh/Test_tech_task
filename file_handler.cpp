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
	m_start(0),
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
	std::error_code errCode;
	m_fileSize = std::filesystem::file_size(m_filePath, errCode);
	if (errCode) {
		m_fileSize = 0;
	}
}

FileHandler::~FileHandler() {
	if(m_file->is_open())
		m_file->close();
}

void FileHandler::open(size_t length) {
	auto newLength = mapped_file::max_length;

	switch(m_mode) {
		case mapped_file_base::mapmode::readonly:
			// Пытаемся открыть на чтение весь файл,
			// если не удаётся, то уменьшаем размер вдвое и пытаемся ещё раз.
			// И так пока запрашиваемый размер больше или равен размеру блока
			do {
				try {
					m_file->open(
						m_filePath,
						m_mode,
						newLength
					);
				} catch(std::exception&e) {
					
				}
				newLength /= 2;
			} while(!m_file->is_open() && newLength >= m_blockSize);
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
//
//bool FileHandler::isWholeFile() const {
//	return (m_start+m_file->size() == m_fileSize);
//}
//
//bool FileHandler::hasNextMap(size_t pos) const {
//	return m_fileSize > (m_start+pos);
//}

void FileHandler::nextMap(size_t pos) {
	if(m_start + pos >= m_fileSize) {
		const auto newStart = m_start + (pos & ~mapped_file::alignment());
		if(m_start != newStart) {
			m_start = newStart;
			m_file->close();
			auto newSize = m_fileSize - m_start;
			do {
				try {
					m_file->open(m_filePath, m_mode, newSize, m_start);
				} catch(std::exception& e) {

				}
				newSize /= 2;
			} while(!m_file->is_open() && newSize >= m_blockSize);
			if(!m_file->is_open()) {
				throw std::invalid_argument("Can't open file " + m_filePath); 
			}
		}
	} else {
		throw std::overflow_error("File buffer wrong shift value " + m_filePath);
	}
}
