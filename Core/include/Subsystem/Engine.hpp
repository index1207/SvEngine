#pragma once

class Functor;

class SVENGINE_API Engine {
    enum EngineOption
    {
        WaitTime = 10,
    };
    using FunctorQueue = ConcurrencyPriorityQueue<std::shared_ptr<Functor>>;
public:
    Engine();
    ~Engine();
public:
    void Initialize();

    void Fetch();
    void Run(int32 io);

    void EnqueueFunctor(const std::shared_ptr<Functor>& functor);
    void EnqueueDbFunctor(const std::shared_ptr<Functor>& functor);
private:
    void ExecuteIo(int32 count);
private:
    FunctorQueue m_dbFunctorQue;
    FunctorQueue m_functorQue;
};

extern Engine* GEngine;