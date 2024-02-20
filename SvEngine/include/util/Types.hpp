#pragma once

#include <ppl.h>
#include <concurrent_vector.h>
#include <concurrent_queue.h>
#include <concurrent_priority_queue.h>
#include <concurrent_unordered_map.h>
#include <concurrent_unordered_set.h>

using int8 = char;
using int16 = short;
using int32 = int;
using int64 = long long;
using uint8 = unsigned char;
using uint16 = unsigned short;
using uint32 = unsigned int;
using uint64 = unsigned long long;

template<class T>
using Vector = std::vector<T>;

template<class T>
using List = std::list<T>;

template<class T>
using Stack = std::stack<T>;

template<class T>
using Queue = std::stack<T>;

template<class T>
using ConcurrencyVector = concurrency::concurrent_vector<T>;

template<class T>
using ConcurrencyQueue = concurrency::concurrent_queue<T>;

template<class T>
using ConcurrencyPriorityQueue = concurrency::concurrent_priority_queue<T>;

template<class Key, class Value>
using Map = std::map<Key, Value>;

template<class Key, class Value>
using HashMap = std::unordered_map<Key, Value>;

template<class Key, class Value>
using ConcurrencyHashMap = concurrency::concurrent_unordered_map<Key, Value>;

template<class Value>
using Set = std::set<Value>;

template<class Value>
using HashSet = std::unordered_set<Value>;

template<class Value>
using ConcurrencyHashSet = concurrency::concurrent_unordered_set<Value>;

using String = std::wstring;
using StringView = std::wstring_view;

#define ASSERT_CRASH(exp) if(!(exp)) { int* ptr = nullptr; *ptr = 1234; }
#define MAKE_LOG_CATEGORY(name) static String Log##name(L#name)