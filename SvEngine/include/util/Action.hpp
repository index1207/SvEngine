#pragma once

#include <random>
#include <sstream>

namespace action
{
	static std::random_device              rd;
	static std::mt19937                    gen(rd());
	static std::uniform_int_distribution<> dis(0, 15);
	static std::uniform_int_distribution<> dis2(8, 11);

	String ToUnicodeString(std::string_view str);

	std::string ToAnsiString(StringView str);

	String UUIDv4() noexcept;

	String Timestamp();

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
