#pragma once

#include "Types.hpp"
#include <sstream>

String ToUnicodeString(std::string_view str);

std::string ToAnsiString(StringView str);

String UUIDv4() noexcept;

String Timestamp();

Vector<String> Split(StringView str, WCHAR del);