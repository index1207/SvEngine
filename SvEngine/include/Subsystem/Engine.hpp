#pragma once

class ThreadManager;
class JobQueue;
class DBConnectionPool;
class JobTimer;

class Engine {
    enum EngineOption
    {
        GQCSTimeout = 10,
        FlushTick = 32
    };
public:
    Engine();
    ~Engine();
public:
    void Initialize();

    void ExecuteThread(int32 io, int32 logic);
    void AddJobQueue(class JobQueue* jobQue);

    /// <summary> (unsafe) PushJob </summary>
    void PushJob(std::shared_ptr<class Job> job);
public:
    __forceinline ThreadManager* GetThreadManager() { return m_threadManager; }
    __forceinline DBConnectionPool* GetDBConnectionPool() { return m_dbConnectionPool; }
    __forceinline JobTimer* GetJobTimer() { return m_jobTimer; }
private:
    void ExecuteLogic(int32 threadCount);
    void ExecuteIo(int32 threadCount);
private:
    ThreadManager* m_threadManager = nullptr;
    DBConnectionPool* m_dbConnectionPool = nullptr;
    JobTimer* m_jobTimer = nullptr;
    ConcurrencyVector<class JobQueue*> m_jobQues;
};

extern Engine* GEngine;