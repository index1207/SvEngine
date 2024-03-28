#include "pch.h"
#include "Util/Action.hpp"
#include <iomanip>
#include <sstream>

String action::ToUnicodeString(std::string_view str)
{
	String ws;
	ws.assign(str.begin(), str.end());
	return ws;
}

std::string action::ToAnsiString(StringView str)
{
	std::string s;
	s.assign(str.begin(), str.end());
	return s;
}

String action::UUIDv4() noexcept
{
	std::wstringstream ss;
	int i;
	ss << std::hex;
	for (i = 0; i < 8; i++) {
		ss << dis(gen);
	}
	ss << "-";
	for (i = 0; i < 4; i++) {
		ss << dis(gen);
	}
	ss << "-4";
	for (i = 0; i < 3; i++) {
		ss << dis(gen);
	}
	ss << "-";
	ss << dis2(gen);
	for (i = 0; i < 3; i++) {
		ss << dis(gen);
	}
	ss << "-";
	for (i = 0; i < 12; i++) {
		ss << dis(gen);
	};
	return ss.str();
}

String action::Timestamp()
{
	auto now = std::time(nullptr);
	tm time = {};

	ASSERT_CRASH(!localtime_s(&time, &now));
	std::wstringstream wss;
	wss << std::format(TEXT("{}/{:02}/{:02} ({:02}:{:02}:{:02})"), time.tm_year + 1900, time.tm_mon, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec);
	return wss.str();
}

Vector<String> action::Split(StringView str, WCHAR del)
{
	Vector<String> res;
	std::wstringstream wss(str.data());

	String t = L"";
	while (std::getline(wss, t, del))
	{
		res.push_back(t);
	}
	return res;
}
