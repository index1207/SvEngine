#pragma once

#include <random>

namespace action
{
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
