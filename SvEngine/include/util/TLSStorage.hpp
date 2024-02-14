#pragma once

extern thread_local uint16 LThreadId;
extern thread_local class JobSerializer* LCurrentJobQueue;
extern thread_local uint64 LEndTickCount;