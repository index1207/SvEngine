#pragma once

class DbConnection;

class DbConnectionPool
{
public:
	DbConnectionPool();
	~DbConnectionPool();

	bool Connect(int32 connectionCount, std::string_view connectionString);
	void Clear();

	DbConnection* Pop();
	void Push(DbConnection* connection);
private:
	std::mutex m_mtx;
	SQLHENV m_env;
	Vector<DbConnection*> m_connections;
};