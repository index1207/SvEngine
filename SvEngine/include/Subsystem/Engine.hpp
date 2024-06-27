#pragma once

class ThreadManager;
class JobSerializer;
class DBConnectionPool;
class JobTimer;

class Engine {
    enum EngineOption
    {
        WorkTick = 1
    };
public:
    Engine();
    ~Engine();
public:
    void Initialize();
    void Polling();

    void AddSerializer(JobSerializer* serializer);
    void ExecuteThread(int32 io, int32 logic, bool enableMainThrd = true);
public:
    __forceinline ThreadManager* GetThreadManager() { return m_threadManager; }
    __forceinline DBConnectionPool* GetDBConnectionPool() { return m_dbConnectionPool; }
    __forceinline JobTimer* GetJobTimer() { return m_jobTimer; }
private:
    void ExecuteLogic(int32 threadCount, std::function<void()> tlsInit = [] {});
    void ExecuteIo(int32 threadCount);
private:
    ThreadManager* m_threadManager = nullptr;
    DBConnectionPool* m_dbConnectionPool = nullptr;
    JobTimer* m_jobTimer = nullptr;

    ConcurrencyQueue<JobSerializer*> m_serializerQue;
};

extern Engine* GEngine;