#pragma once

class ThreadManager;
class JobQueue;
class DBConnectionPool;
class JobTimer;

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
    __forceinline JobTimer* GetJobTimer() { return m_jobTimer; }
private:
    void ExecuteWorker();
    void DistributeDeferredJob();
private:
    ThreadManager* m_threadManager;
    JobQueue* m_jobQue;
    DBConnectionPool* m_dbConnectionPool;
    JobTimer* m_jobTimer;
};

extern Engine* GEngine;