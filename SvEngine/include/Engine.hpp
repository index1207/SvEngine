#pragma once

class ThreadManager;
class JobQueue;
class DBManager;

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
    void HandleError(LogCategory category);
public:
    __forceinline ThreadManager* GetThreadManager() { return m_threadManager; }
    __forceinline JobQueue* GetJobQueue() { return m_jobQue; }
    __forceinline DBManager* GetDBManager() { return m_dbManager; }
private:
    void ExecuteWorker();
private:
    ThreadManager* m_threadManager;
    JobQueue* m_jobQue;
    DBManager* m_dbManager;
};

extern Engine* GEngine;