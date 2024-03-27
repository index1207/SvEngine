#include "pch.h"
#include "Util/Parser/Ini.hpp"

Ini::Ini(StringView filename) : m_filename(filename)
{

}

Ini::Section Ini::operator[](StringView section)
{
	return Section(m_filename, section.data());
}

Ini::Section::Section(StringView filename, StringView section) : m_filename(filename), m_section(section)
{
}
