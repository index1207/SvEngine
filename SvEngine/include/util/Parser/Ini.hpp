#pragma once

#include "CoreHeader.hpp"

class Ini
{
	class Section
	{
	public:
		Section(StringView filename, StringView section);
	public:
		template<class T>
		T Get(StringView key)
		{
			T buffer;
			GetPrivateProfileStruct(m_section.c_str(), key.data(), &buffer, sizeof(buffer), m_filename.c_str());
			return buffer;
		}
		template<>
		int Get<int>(StringView key)
		{
			return GetPrivateProfileInt(m_section.c_str(), key.data(), 0, m_filename.c_str());
		}
		template<>
		String Get<String>(StringView key)
		{
			WCHAR buffer[1024] = L"";
			GetPrivateProfileString(m_section.c_str(), key.data(), L"", buffer, 1024, m_filename.c_str());
			return buffer;
		}
	private:
		String m_section;
		String m_filename;
	};
public:
	Ini(StringView filename);
public:
	Section operator[](StringView section);
private:
	String m_filename;
};