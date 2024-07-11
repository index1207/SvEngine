#pragma once
#include <sstream>
#include <future>

#include "cppconn/driver.h"
#include "cppconn/prepared_statement.h"
#include "cppconn/resultset.h"

#include "Functor.hpp"

class  AsyncStatement :
    public std::enable_shared_from_this<AsyncStatement>,
    public sql::PreparedStatement
{
public:
    void AsyncExecute(std::function<void(bool)> callback)
    {
        auto sharedThis = shared_from_this();
        GEngine->EnqueueDbFunctor(MakeShared<Functor>([sharedThis, callback] {
            auto ret = sharedThis->execute();
            callback(ret);
        }));
    }

    void AsyncUpdate(std::function<void(int)> callback)
    {
        m_sharedThis = shared_from_this();
        GEngine->EnqueueDbFunctor(MakeShared<Functor>([=, this] {
            auto ret = m_sharedThis->executeUpdate();
            callback(ret);
            m_sharedThis = nullptr;
        }));
    }

    void AsyncQuery(std::function<void(std::unique_ptr<sql::ResultSet>)> callback)
    {
        auto sharedThis = shared_from_this();
        GEngine->EnqueueDbFunctor(MakeShared<Functor>([sharedThis, callback] {
            auto ret = std::unique_ptr<sql::ResultSet>(sharedThis->executeQuery());
            callback(std::move(ret));
        }));
    }
private:
    std::shared_ptr<AsyncStatement> m_sharedThis;
};

class SVENGINE_API Database
{
public:
    Database();
public:
    void Initialize();
public:
    void SetDatabaseProfile(StringView username, StringView password, StringView dbname);
    void CreateConnection();

    template<class... Args>
    std::shared_ptr<AsyncStatement> CallProcedure(const char* procedure, Args... args)
    {
        if (m_connections.empty())
        {
            m_maxConnectionCount *= 2;
            CreateConnection();
        }

        std::shared_ptr<sql::Connection> conn = m_connections.front();
        m_connections.pop_front();

        auto pstmt = std::shared_ptr<AsyncStatement>(
            static_cast<AsyncStatement*>(conn->prepareStatement(
                std::format("CALL {}({})",
                    procedure, concatenate(args...)
                )
            ))
        );
        return pstmt;
    }
private:
    template <typename T>
    struct is_string
        : std::false_type
    {};

    template <typename CharT, typename TraitsT, typename AllocT>
    struct is_string<std::basic_string<CharT, TraitsT, AllocT>>
        : std::true_type
    {};

    template<typename T, typename... Args>
    void concatArgs(std::stringstream& ss, T first, Args... args) {
        if constexpr (is_string<T>::value)
            first = std::format("'{}'", first);
        ss << first;
        if constexpr (sizeof...(args) > 0) {
            ss << ", ";
            concatArgs(ss, args...);
        }
    }

    template<typename... Args>
    std::string concatenate(Args... args) {
        std::stringstream ss;
        concatArgs(ss, args...);
        return ss.str();
    }
private:
    String m_dbUserName;
    String m_dbPwd;
    String m_dbName;
    uint32 m_maxConnectionCount = 10;
    sql::Driver* m_driver;
    List<std::shared_ptr<sql::Connection>> m_connections;
};

extern Database* GDatabase;