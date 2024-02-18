#pragma once
#include "DBConnection.hpp"

class DBConnectionPool
{
public:
	DBConnectionPool();
	~DBConnectionPool();

	bool Connect(int32 connectionCount, std::wstring_view connectionString);
	void Clear();

	DBConnection* Pop();
	void Push(DBConnection* connection);
private:
	std::mutex m_mtx;
	SQLHENV _environment = SQL_NULL_HANDLE;
	Vector<DBConnection*> _connections;
};

