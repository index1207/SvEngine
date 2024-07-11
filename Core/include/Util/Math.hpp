#pragma once

class SVENGINE_API Math
{
public:
	static float Round(float x, int32 p = 0);
	static double Round(double x, int32 p = 0);
	
	template<class T>
	static T Abs(T value)
	{
		return value < 0 ? -value : value;
	}
};

