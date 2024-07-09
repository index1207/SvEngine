#pragma once

#define DLLEXPORT __declspec(dllexport)

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