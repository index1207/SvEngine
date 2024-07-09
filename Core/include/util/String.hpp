#pragma once

#include "Types.hpp"
#include <sstream>

DLLEXPORT String ToUnicodeString(std::string_view str);

DLLEXPORT std::string ToAnsiString(StringView str);

DLLEXPORT String UUIDv4() noexcept;

DLLEXPORT String Timestamp();

DLLEXPORT Vector<String> Split(StringView str, WCHAR del);