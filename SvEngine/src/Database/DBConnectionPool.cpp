#include "pch.h"
#include "Database/DBConnectionPool.hpp"

DBConnectionPool::DBConnectionPool()
{

}

DBConnectionPool::~DBConnectionPool()
{
	Clear();
}

bool DBConnectionPool::Connect(int32 connectionCount, String connectionString)
{
	if (::SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &_environment) != SQL_SUCCESS)
		return false;

	if (::SQLSetEnvAttr(_environment, SQL_ATTR_ODBC_VERSION, reinterpret_cast<SQLPOINTER>(SQL_OV_ODBC3), 0) != SQL_SUCCESS)
		return false;

	for (int32 i = 0; i < connectionCount; i++)
	{
		DBConnection* connection = new DBConnection;
		if (connection->Connect(_environment, connectionString) == false)
			return false;

		_connections.push(connection);
	}

	return true;
}

void DBConnectionPool::Clear()
{
	if (_environment != SQL_NULL_HANDLE)
	{
		::SQLFreeHandle(SQL_HANDLE_ENV, _environment);
		_environment = SQL_NULL_HANDLE;
	}

	while (_connections.empty())
	{
		DBConnection* connection = nullptr;
		if (_connections.try_pop(connection))
			delete connection;
	}

	_connections.clear();
}

DBConnection* DBConnectionPool::Pop()
{
	if (_connections.empty())
		return nullptr;

	DBConnection* connection = nullptr;
	while (!_connections.try_pop(connection)) {};
	return connection;
}

void DBConnectionPool::Push(DBConnection* connection)
{
	_connections.push(connection);
}
