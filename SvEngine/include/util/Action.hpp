#pragma once

#include <random>

namespace action
{
	inline String ToUnicodeString(std::string_view str)
	{
		String ws;
		ws.assign(str.begin(), str.end());
		return ws;
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
