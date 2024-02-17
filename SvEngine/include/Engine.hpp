#pragma once

class ThreadManager;
class JobQueue;
class DBConnectionPool;

class Engine {
    enum Define
    {
        WORK_TICK = 64
    };
public:
    Engine();
    ~Engine();
public:
    void ExecuteIocpLogic(int32 threadCount, bool useMainThrd);
public:
    __forceinline ThreadManager* GetThreadManager() { return m_threadManager; }
    __forceinline JobQueue* GetJobQueue() { return m_jobQue; }
    __forceinline DBConnectionPool* GetDBConnectionPool() { return m_dbConnectionPool; }
private:
    void ExecuteWorker();
private:
    ThreadManager* m_threadManager;
    JobQueue* m_jobQue;
    DBConnectionPool* m_dbConnectionPool;
};

extern Engine* GEngine;