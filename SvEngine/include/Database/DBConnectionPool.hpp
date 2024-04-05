#pragma once
#include "DBConnection.hpp"

class DBConnectionPool
{
public:
	DBConnectionPool();
	~DBConnectionPool();

	bool Connect(int32 connectionCount, String connectionString);
	void Clear();

	DBConnection* Pop();
	void Push(DBConnection* connection);
private:
	SQLHENV _environment = SQL_NULL_HANDLE;
	ConcurrencyQueue<DBConnection*> _connections;
};

