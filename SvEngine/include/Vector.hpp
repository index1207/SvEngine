#pragma once

template<class T>
class Vector2D
{
public:
	explicit Vector2D(T x, T y) noexcept
		: m_x(x), m_y(y)
	{
	}
	~Vector2D() = default;
public:
	void SetX(T x) noexcept { m_x = x; }
	void SetY(T y) noexcept { m_y = y; }
	T GetX() const noexcept { return m_x; }
	T GetY() const noexcept { return m_y; }
public:
#pragma region Linear Algebra
	double Length() const { return std::sqrt(m_x * m_x + m_y * m_y); }
	void Normalize()
	{
		const auto len = Length();
		return Vector2D<T>(m_x / len, m_y / len);
	}
	void operator+=(Vector2D<T>&& v) noexcept
	{
		m_x += v.m_x;
		m_y += v.m_y;
	}
	void operator-=(Vector2D<T>&& v) noexcept
	{
		m_x -= v.m_x;
		m_y -= v.m_y;
	}
	void operator+=(const Vector2D<T>& v) noexcept
	{
		m_x += v.m_x;
		m_y += v.m_y;
	}
	void operator-=(const Vector2D<T>& v) noexcept
	{
		m_x -= v.m_x;
		m_y -= v.m_y;
	}
	void operator*=(double scala) noexcept
	{
		m_x *= scala;
		m_y *= scala;
	}
	Vector2D<T> operator+(Vector2D<T>&& v) noexcept
	{
		return Vector2D<T>(
			m_x + v.m_x,
			m_y + v.m_y
		);
	}
	Vector2D<T> operator+(const Vector2D<T>& v) noexcept
	{
		return Vector2D<T>(
			m_x + v.m_x,
			m_y + v.m_y
		);
	}
	Vector2D<T> operator-(Vector2D<T>&& v) noexcept
	{
		return Vector2D<T>(
			m_x - v.m_x,
			m_y - v.m_y
		);
	}
	Vector2D<T> operator-(const Vector2D<T>& v) noexcept
	{
		return Vector2D<T>(
			m_x - v.m_x,
			m_y - v.m_y
		);
	}
	Vector2D<T> operator*(double scala) noexcept
	{
		return VectorD<T>(
			m_x * scala,
			m_y * scala
		);
	}
#pragma endregion
public:
	static Vector2D<T> Zero() noexcept { return Vector2D<T>(0, 0); }
	static Vector2D<T> One() noexcept { return Vector2D<T>(1, 1); }
	static Vector2D<T> Up() noexcept { return Vector2D<T>(0, 1); }
	static Vector2D<T> Down() noexcept { return Vector2D<T>(0, -1); }
	static Vector2D<T> Left() noexcept { return Vector2D<T>(-1, 0); }
	static Vector2D<T> Right() noexcept { return Vector2D<T>(1, 0); }
private:
	T m_x;
	T m_y;
};

template<class T>
class Vector3D
{
public:
	explicit Vector3D(T x, T y, T z) noexcept
		: m_x(x), m_y(y), m_z(z)
	{
	}
	~Vector3D() = default;
public:
	void SetX(T x) noexcept { m_x = x; }
	void SetY(T y) noexcept { m_y = y; }
	void SetZ(T z) noexcept { m_y = z; }
	T GetX() noexcept { return m_x; }
	T GetY() noexcept { return m_y; }
	T GetZ() noexcept { return m_z; }
public:
#pragma region Linear Algebra
	double Length() const { return std::sqrt(m_x * m_x + m_y * m_y + m_z * m_z); }
	void Normalize()
	{
		const auto len = Length();
		return Vector3D<T>(m_x / len, m_y / len, m_z / len);
	}
	void operator+=(Vector3D<T>&& v) noexcept
	{
		m_x += v.m_x;
		m_y += v.m_y;
		m_z += v.m_z;
	}
	void operator-=(Vector3D<T>&& v) noexcept
	{
		m_x -= v.m_x;
		m_y -= v.m_y;
		m_z -= v.m_z;
	}
	void operator+=(const Vector3D<T>& v) noexcept
	{
		m_x += v.m_x;
		m_y += v.m_y;
		m_z += v.m_z;
	}
	void operator-=(const Vector3D<T>& v) noexcept
	{
		m_x -= v.m_x;
		m_y -= v.m_y;
		m_z -= v.m_z;
	}
	void operator*=(double scala) noexcept
	{
		m_x *= scala;
		m_y *= scala;
		m_z *= scala;
	}
	Vector3D<T> operator+(Vector3D<T>&& v) noexcept
	{
		return Vector3D<T>(
			m_x + v.m_x,
			m_y + v.m_y,
			m_z + v.m_z
		);
	}
	Vector3D<T> operator+(const Vector3D<T>& v) noexcept
	{
		return Vector3D<T>(
			m_x + v.m_x,
			m_y + v.m_y,
			m_z + v.m_z
		);
	}
	Vector3D<T> operator-(Vector3D<T>&& v) noexcept
	{
		return Vector3D<T>(
			m_x - v.m_x,
			m_y - v.m_y,
			m_z - v.m_z
		);
	}
	Vector3D<T> operator-(const Vector3D<T>& v) noexcept
	{
		return Vector3D<T>(
			m_x - v.m_x,
			m_y - v.m_y,
			m_z - v.m_z
		);
	}
	Vector3D<T> operator*(double scala) noexcept
	{
		return Vector3D<T>(
			m_x * scala,
			m_y * scala,
			m_z * scala
		);
	}
#pragma endregion
public:
	static Vector3D<T> Zero() noexcept { return Vector3D<T>(0, 0, 0); }
	static Vector3D<T> One() noexcept { return Vector3D<T>(1, 1, 1); }
	static Vector3D<T> Up() noexcept { return Vector3D<T>(0, 0, 1); }
	static Vector3D<T> Down() noexcept { return Vector3D<T>(0, 0, -1); }
	static Vector3D<T> Left() noexcept { return Vector3D<T>(0, -1, 0); }
	static Vector3D<T> Right() noexcept { return Vector3D<T>(0, 1, 0); }
	static Vector3D<T> Foward() noexcept { return Vector3D<T>(1, 0, 0); }
	static Vector3D<T> Back() noexcept { return Vector3D<T>(-1, 0, 0); }
private:
	T m_x;
	T m_y;
	T m_z;
};

using Vector3DI = Vector3D<int>;
using Vector3DF = Vector3D<float>;