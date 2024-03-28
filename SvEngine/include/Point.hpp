#pragma once

template<class T>
class Point2D
{
public:
	explicit Point2D(T x, T y) noexcept
		: m_x(x), m_y(y)
	{
	}
public:
	void SetX(T x) noexcept { m_x = x; }
	void SetY(T y) noexcept { m_y = y; }
	const T GetX() noexcept { return m_x; }
	const T GetY() noexcept { return m_y; }
private:
	T m_x;
	T m_y;
};

template<class T>
class Point3D
{
public:
	explicit Point3D(T x, T y, T z) noexcept
		: m_x(x), m_y(y), m_z(z)
	{
	}
public:
	void SetX(T x) noexcept { m_x = x; }
	void SetY(T y) noexcept { m_y = y; }
	void SetZ(T z) noexcept { m_y = z; }
	const T GetX() noexcept { return m_x; }
	const T GetY() noexcept { return m_y; }
	const T GetZ() noexcept { return m_z; }
private:
	T m_x;
	T m_y;
	T m_z;
};

using Point2DI = Point2D<int>;
using Point2DF = Point2D<float>;