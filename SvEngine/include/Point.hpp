#pragma once

template<class T>
class Point2D
{
public:
	explicit Point2D(T x, T y) noexcept
		: x(x), y(y)
	{
	}
public:
	T x;
	T y;
};

template<class T>
class Point3D
{
public:
	explicit Point3D(T x, T y, T z) noexcept
		: x(x), y(y), z(z)
	{
	}
public:
	T x;
	T y;
	T z;
};

using Point2DI = Point2D<int>;
using Point2DF = Point2D<float>;