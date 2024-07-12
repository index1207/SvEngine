#include "pch.h"
#include "Util/File.hpp"

#include <fstream>

File::File(StringView path) : m_path(path)
{
}

File::~File()
{
}

Vector<BYTE> File::Read()
{
	Vector<BYTE> buffer;
	const auto size = fs::file_size(m_path);
	buffer.resize(size);

	std::basic_ifstream<BYTE> file(m_path);
	file.read(&buffer[0], size);

	return buffer;
}

void File::Write(StringView data)
{
	std::wofstream file(m_path);
	file << data;
}
