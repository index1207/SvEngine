#include <pch.h>
#include <util/TLSStorage.hpp>

thread_local uint16 LThreadId = 0;
thread_local JobSerializer* LCurrentJobQueue = nullptr;
thread_local uint64 LEndTickCount = 0;