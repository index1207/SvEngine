#pragma once

#include <vector>
#include <array>
#include <queue>
#include <stack>
#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>

#include <tbb/concurrent_vector.h>
#include <tbb/concurrent_queue.h>
#include <tbb/concurrent_priority_queue.h>
#include <tbb/concurrent_unordered_map.h>
#include <tbb/concurrent_unordered_set.h>

using int8 = char;
using int16 = short;
using int32 = int;
using int64 = long long;
using uint8 = unsigned char;
using uint16 = unsigned short;
using uint32 = unsigned int;
using uint64 = unsigned long long;

template<class T, class Allocator = std::allocator<T>>
using Vector = std::vector<T, Allocator>;

template<class T, size_t N>
using Array = std::array<T, N>;

template<class T>
using List = std::list<T>;

template<class T>
using Stack = std::stack<T>;

template<class T>
using Queue = std::queue<T>;

template<class T>
using PriorityQueue = std::priority_queue<T>;

template<class T>
using ConcurrencyVector = tbb::concurrent_vector<T>;

template<class T>
using ConcurrencyQueue = tbb::concurrent_queue<T>;

template<class T, class Compare = std::less<T>>
using ConcurrencyPriorityQueue = tbb::concurrent_priority_queue<T, Compare>;

template<class Key, class Value>
using Map = std::map<Key, Value>;

template<class Key, class Value>
using HashMap = std::unordered_map<Key, Value>;

template<class Key, class Value>
using ConcurrencyHashMap = tbb::concurrent_unordered_map<Key, Value>;

template<class Value>
using Set = std::set<Value>;

template<class Value>
using HashSet = std::unordered_set<Value>;

template<class Value>
using ConcurrencyHashSet = tbb::concurrent_unordered_set<Value>;

using String = std::wstring;
using StringView = std::wstring_view;

#define CRASH() \
{ \
	int* crash = nullptr; \
	__analysis_assume(crash != nullptr); \
	*crash = 0xDEAD; \
}
#define ASSERT_CRASH(expr) \
{ \
	if(!(expr)) \
	{ \
		CRASH(); \
		__analysis_assume(expr); \
	} \
}
#define MAKE_LOG_CATEGORY(name) namespace Category { static String name(L#name); }