#pragma once

class Functor;

class DLLEXPORT Engine {
    enum EngineOption
    {
        WaitTime = 10
    };
public:
    Engine();
    ~Engine();
public:
    void Initialize();

    void Fetch();
    void Run(int32 io);

    void EnqueueFunctor(const std::shared_ptr<Functor>& functor);
private:
    void ExecuteIo(int32 threadCount);
private:
    ConcurrencyPriorityQueue<std::shared_ptr<Functor>> m_functorQue;
};

extern Engine* GEngine;