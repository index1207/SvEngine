#pragma once

#include <type_traits>
#include <memory>

template<class T>
struct PeelArrayType { using type = T; };
template<class T>
struct PeelArrayType<T[]> { using type = T; };

template<class T>
struct PeelPointerType { using type = T; };
template<class T>
struct PeelPointerType<T*> { using type = T; };

template<class T, class... Args> requires std::is_class_v<T>
static inline std::shared_ptr<T> MakeShared(Args&&... args)
{
	return std::shared_ptr<T>(new T(std::forward<Args>(args)...));
}
template<class T> requires std::is_unbounded_array_v<T>
static inline std::shared_ptr<T> MakeShared(const size_t size)
{
	using Ty = PeelArrayType<T>::type;
	return std::shared_ptr<T>(new Ty[size]);
}

template<class T, class... Args> requires std::is_class_v<T>
static inline std::unique_ptr<T> MakeUnique(Args&&... args)
{
	return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}
template<class T> requires std::is_unbounded_array_v<T>
static inline std::unique_ptr<T> MakeUnique(const size_t size)
{
	using Ty = PeelArrayType<T>::type;
	return std::unique_ptr<T>(new Ty[size]);
}