#include "pch.h"
#include "Util/Math.hpp"

float Math::Round(float x, int32 p)
{
	return roundf(x * static_cast<float>(std::pow(10, p))) / 10.f;
}

double Math::Round(double x, int32 p)
{
	return round(x * std::pow(10., p)) / 10.;
}
