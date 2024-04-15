#include <pch.h>
#include <Thread/TLSStorage.hpp>
#include <Thread/JobSerializer.hpp>

thread_local uint16 LThreadId = 0;
thread_local JobQueue* LJobQueue = nullptr;
thread_local uint64 LEndTick = 0;