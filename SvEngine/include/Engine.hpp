#pragma once

#include "Thread/ThreadManager.hpp"
#include "Thread/JobSerializer.hpp"

class Engine {
    enum Define
    {
        WORK_TICK = 64
    };
public:
    Engine();
    ~Engine();
public:
    void ExecuteIocpLogic();
public:
    __forceinline ThreadManager* GetThreadManager() { return m_threadManager; }
    __forceinline JobQueue* GetJobQueue() { return m_jobQue; }
private:
    ThreadManager* m_threadManager;
    JobQueue* m_jobQue;
};

extern Engine* GEngine;