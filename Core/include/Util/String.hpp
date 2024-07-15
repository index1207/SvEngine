#pragma once

#include "Types.hpp"
#include <sstream>

SVENGINE_API String ToUnicodeString(std::string_view str);

SVENGINE_API std::string ToAnsiString(StringView str);

SVENGINE_API String UUIDv4() noexcept;

SVENGINE_API String Timestamp();

SVENGINE_API Vector<String> Split(StringView str, WCHAR del);