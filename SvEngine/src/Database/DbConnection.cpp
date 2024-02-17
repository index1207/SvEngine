#include "pch.h"
#include "Database/DBConnection.hpp"

#include "mysql.h"

DBConnection::DBConnection(MYSQL* connection) : m_connection(connection)
{
}

DBConnection::~DBConnection()
{
}

void DBConnection::Execute(std::string_view query)
{
	if (mysql_query(m_connection, query.data()))
		GEngine->HandleError(LogMYSQL);
}
