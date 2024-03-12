#pragma once

#include <random>
#include <sstream>

namespace action
{
	static std::random_device              rd;
	static std::mt19937                    gen(rd());
	static std::uniform_int_distribution<> dis(0, 15);
	static std::uniform_int_distribution<> dis2(8, 11);

	inline String ToUnicodeString(std::string_view str)
	{
		String ws;
		ws.assign(str.begin(), str.end());
		return ws;
	}

	inline std::string ToAnsiString(StringView str)
	{
		std::string s;
		s.assign(str.begin(), str.end());
		return s;
	}

	inline String generate_uuid_v4() {
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

	class Random
	{
	public:
		template<typename T>
		static T RandomRange(T min, T max)
		{
			std::random_device rd;
			std::mt19937 gen(rd());

			if constexpr (std::is_integral_v<T>)
			{
				std::uniform_int_distribution<T> dist(min, max);
				return dist(gen);
			}
			else
			{
				std::uniform_real_distribution<T> dist(min, max);
				return dist(gen);
			}
		}
	};
}
