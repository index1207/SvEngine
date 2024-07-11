#pragma once
#include <sstream>
#include <future>

#include "cppconn/driver.h"
#include "cppconn/prepared_statement.h"
#include "cppconn/resultset.h"

#include "Functor.hpp"

class AsyncStatement;

class SVENGINE_API Database
{
    friend AsyncStatement;
public:
    Database();
public:
    void Initialize();
public:
    void SetDatabaseProfile(StringView username, StringView password, StringView dbname);
    void CreateConnection();

    template<class... Args>
    std::unique_ptr<AsyncStatement> CreateStatement(const char* formattedSql, Args... args)
    {
        auto conn = PopConnection();

        auto pstmt = std::shared_ptr<AsyncStatement>(
            static_cast<AsyncStatement*>(conn->prepareStatement(
                std::format(formattedSql, args...)
            ))
        );
        return pstmt;
    }

    template<class... Args>
    std::unique_ptr<AsyncStatement> CallProcedure(const char* procedure, Args... args)
    {
        auto conn = PopConnection();

        auto pstmt = std::unique_ptr<AsyncStatement>(
            static_cast<AsyncStatement*>(conn->prepareStatement(
                std::format("CALL {}({})",
                    procedure, concatenate(args...)
                )
            ))
        );
        return pstmt;
    }
private:
    std::shared_ptr<sql::Connection> PopConnection();
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

class AsyncStatement : public sql::PreparedStatement
{
public:
    std::future<bool> AsyncExecute(std::function<void(bool)> callback = [](bool) {})
    {
        return std::async([this, callback] {
            try
            {
                auto res = this->execute();
                callback(res);
                return res;
            }
            catch (std::exception& e)
            {
                Console::Error(Category::Database, ToUnicodeString(e.what()));
            }
        });
    }

    std::future<int> AsyncUpdate(std::function<void(int)> callback = [](int) {})
    {
        return std::async([this, callback] {
            try
            {
                auto res = this->executeUpdate();
                callback(res);
                return res;
            }
            catch (std::exception& e)
            {
                Console::Error(Category::Database, ToUnicodeString(e.what()));
            }
        });
    }

    std::future<std::shared_ptr<sql::ResultSet>> AsyncQuery(std::function<void(std::shared_ptr<sql::ResultSet>)> callback =
        [](std::shared_ptr<sql::ResultSet>) {})
    {
        return std::async([this, callback] {
            try
            {
                auto res = std::shared_ptr<sql::ResultSet>(this->executeQuery());
                callback(res);
                return res;
            }
            catch (std::exception& e)
            {
                Console::Error(Category::Database, ToUnicodeString(e.what()));
            }
        });
    }
};