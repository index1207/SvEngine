#include "pch.h"
#include "Database/DBManager.hpp"
#include "Database/DBConnection.hpp"

#include "mysql.h"

DBManager::DBManager()
{
	m_handle = new MYSQL;
	if (mysql_init(m_handle) == nullptr)
		GEngine->HandleError(LogMYSQL);
}

DBManager::~DBManager()
{
	mysql_close(m_handle);
	Clear();
	
	delete m_handle;
}

bool DBManager::Connect(int32 count, ConnectionData data)
{
	for (int32 i = 0; i < count; ++i)
	{
		auto conn = mysql_real_connect(m_handle,
			data.host.c_str(),
			data.user.c_str(),
			data.password.data(),
			data.database.c_str(),
			data.port,
			nullptr, NULL
		);
		if (conn == nullptr)
		{
			GEngine->HandleError(LogMYSQL);
			return false;
		}

		if (mysql_select_db(conn, data.database.c_str()))
		{
			GEngine->HandleError(LogMYSQL);
			return false;
		}

		auto dbConn = new DBConnection(conn);
		m_connections.push_back(dbConn);
	}
	return true;
}

void DBManager::Clear()
{
	for (auto* conn : m_connections)
		delete conn;
	m_connections.clear();
}