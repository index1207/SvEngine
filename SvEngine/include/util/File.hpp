#pragma once

#include <filesystem>

namespace fs = std::filesystem;

class File
{
public:
	File(StringView path);
	~File();
public:
	Vector<BYTE> Read();
	void Write(StringView data);
private:
	fs::path m_path;
};

