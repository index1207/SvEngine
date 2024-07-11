#pragma once

template<class T>
class SVENGINE_API Vector2D
{
public:
	Vector2D() = default;
	explicit Vector2D(T x, T y) noexcept
		: x(x), y(y)
	{
	}
	~Vector2D() = default;
public:
#pragma region Linear Algebra
	double Length() const { return std::sqrt(x * x + y * y); }
	Vector2D<T> Normalize()
	{
		const auto len = Length();
		return Vector2D<T>(static_cast<T>(x / len), static_cast<T>(y / len));
	}
	void operator+=(Vector2D<T>&& v) noexcept
	{
		x += v.x;
		y += v.y;
	}
	void operator-=(Vector2D<T>&& v) noexcept
	{
		x -= v.x;
		y -= v.y;
	}
	void operator+=(const Vector2D<T>& v) noexcept
	{
		x += v.x;
		y += v.y;
	}
	void operator-=(const Vector2D<T>& v) noexcept
	{
		x -= v.x;
		y -= v.y;
	}
	void operator*=(double scala) noexcept
	{
		x *= scala;
		y *= scala;
	}
	Vector2D<T> operator+(Vector2D<T>&& v) noexcept
	{
		return Vector2D<T>(
			x + v.x,
			y + v.y
		);
	}
	Vector2D<T> operator+(const Vector2D<T>& v) noexcept
	{
		return Vector2D<T>(
			x + v.x,
			y + v.y
		);
	}
	Vector2D<T> operator-(Vector2D<T>&& v) noexcept
	{
		return Vector2D<T>(
			x - v.x,
			y - v.y
		);
	}
	Vector2D<T> operator-(const Vector2D<T>& v) noexcept
	{
		return Vector2D<T>(
			x - v.x,
			y - v.y
		);
	}
	Vector2D<T> operator*(double scala) noexcept
	{
		return Vector2D<T>(
			x * scala,
			y * scala
		);
	}
#pragma endregion
	bool operator==(Vector2D<T> v)
	{
		return x == v.x && y == v.y;
	}
public:
	static Vector2D<T> Zero() noexcept { return Vector2D<T>(0, 0); }
	static Vector2D<T> One() noexcept { return Vector2D<T>(1, 1); }
	static Vector2D<T> Up() noexcept { return Vector2D<T>(0, 1); }
	static Vector2D<T> Down() noexcept { return Vector2D<T>(0, -1); }
	static Vector2D<T> Left() noexcept { return Vector2D<T>(-1, 0); }
	static Vector2D<T> Right() noexcept { return Vector2D<T>(1, 0); }
public:
	mutable T x;
	mutable T y;
};

template<class T>
class SVENGINE_API Vector3D
{
public:
	Vector3D() = default;
	explicit Vector3D(T x, T y, T z) noexcept
		: x(x), y(y), z(z)
	{
	}
	~Vector3D() = default;
public:
#pragma region Linear Algebra
	double Length() const { return std::sqrt(x * x + y * y + z * z); }
	void Normalize()
	{
		const auto len = Length();
		return Vector3D<T>(x / len, y / len, z / len);
	}
	void operator+=(Vector3D<T>&& v) noexcept
	{
		x += v.x;
		y += v.y;
		z += v.z;
	}
	void operator-=(Vector3D<T>&& v) noexcept
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
	}
	void operator+=(const Vector3D<T>& v) noexcept
	{
		x += v.x;
		y += v.y;
		z += v.z;
	}
	void operator-=(const Vector3D<T>& v) noexcept
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
	}
	void operator*=(double scala) noexcept
	{
		x *= scala;
		y *= scala;
		z *= scala;
	}
	Vector3D<T> operator+(Vector3D<T>&& v) noexcept
	{
		return Vector3D<T>(
			x + v.x,
			y + v.y,
			z + v.z
		);
	}
	Vector3D<T> operator+(const Vector3D<T>& v) noexcept
	{
		return Vector3D<T>(
			x + v.x,
			y + v.y,
			z + v.z
		);
	}
	Vector3D<T> operator-(Vector3D<T>&& v) noexcept
	{
		return Vector3D<T>(
			x - v.x,
			y - v.y,
			z - v.z
		);
	}
	Vector3D<T> operator-(const Vector3D<T>& v) noexcept
	{
		return Vector3D<T>(
			x - v.x,
			y - v.y,
			z - v.z
		);
	}
	Vector3D<T> operator*(double scala) noexcept
	{
		return Vector3D<T>(
			x * scala,
			y * scala,
			z * scala
		);
	}
#pragma endregion
	bool operator==(Vector3D<T> v)
	{
		return x == v.x && y == v.y && z == v.z;
	}
public:
	static Vector3D<T> Zero() noexcept { return Vector3D<T>(0, 0, 0); }
	static Vector3D<T> One() noexcept { return Vector3D<T>(1, 1, 1); }
	static Vector3D<T> Up() noexcept { return Vector3D<T>(0, 0, 1); }
	static Vector3D<T> Down() noexcept { return Vector3D<T>(0, 0, -1); }
	static Vector3D<T> Left() noexcept { return Vector3D<T>(0, -1, 0); }
	static Vector3D<T> Right() noexcept { return Vector3D<T>(0, 1, 0); }
	static Vector3D<T> Foward() noexcept { return Vector3D<T>(1, 0, 0); }
	static Vector3D<T> Back() noexcept { return Vector3D<T>(-1, 0, 0); }
public:
	T x;
	T y;
	T z;
};

using Vector2DI = Vector2D<int>;
using Vector2DF = Vector2D<float>;

using Vector3DI = Vector3D<int>;
using Vector3DF = Vector3D<float>;