#include "pch.h"
#include "Database/DbConnectionPool.hpp"
#include "Database/DbConnection.hpp"

DbConnectionPool::DbConnectionPool() : m_env(SQL_NULL_HANDLE)
{
}

DbConnectionPool::~DbConnectionPool()
{
	Clear();
}

bool DbConnectionPool::Connect(int32 connectionCount, std::string_view connectionString)
{
	std::lock_guard lock(m_mtx);

	if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_env) != SQL_SUCCESS)
		return false;
	if (SQLSetEnvAttr(m_env, SQL_ATTR_ODBC_VERSION, reinterpret_cast<SQLPOINTER>(SQL_OV_ODBC3), 0))
		return false;

	for (int32 i = 0; i < connectionCount; ++i)
	{
		auto* connection = new DbConnection(m_env);
		if (!connection->Connect(connectionString))
			return false;

		m_connections.push_back(connection);
	}
	return true;
}

void DbConnectionPool::Clear()
{
	std::lock_guard lock(m_mtx);

	if (m_env != SQL_NULL_HANDLE)
	{
		SQLFreeHandle(SQL_HANDLE_ENV, m_env);
		m_env = SQL_NULL_HANDLE;
	}

	for (auto* connection : m_connections)
		delete connection;

	m_connections.clear();
}

DbConnection* DbConnectionPool::Pop()
{
	std::lock_guard lock(m_mtx);

	if (!m_connections.empty())
		return nullptr;

	auto* connection = m_connections.back();
	m_connections.pop_back();
	return connection;
}

void DbConnectionPool::Push(DbConnection* connection)
{
	std::lock_guard lock(m_mtx);
	m_connections.push_back(connection);
}
