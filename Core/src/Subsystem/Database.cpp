#include "pch.h"
#include "Subsystem/Database.hpp"

void Database::CreateConnection()
{
	auto deleter = [this](sql::Connection* conn) {
		m_connections.emplace_back(std::shared_ptr<sql::Connection>(conn));
	};

	for (uint32 i = 0; i < m_maxConnectionCount; ++i)
	{
		try {
			auto conn = std::shared_ptr<sql::Connection>(
				m_driver->connect("tcp://localhost:3306",
					ToAnsiString(m_dbUserName),
					ToAnsiString(m_dbPwd)
				),
				deleter
			);
			conn->setSchema(ToAnsiString(m_dbName));
			m_connections.emplace_back(conn);
		}
		catch (std::exception&) {
			Console::Error(Category::Database, TEXT("Could not connect to specified database profile."));
		}
	}
}

std::shared_ptr<sql::Connection> Database::PopConnection()
{
	if (m_connections.empty())
	{
		m_maxConnectionCount *= 2;
		CreateConnection();
	}

	std::shared_ptr<sql::Connection> conn = m_connections.front();
	m_connections.pop_front();

	return conn;
}

Database::Database()
{
	m_driver = nullptr;
	m_dbUserName = TEXT("");
	m_dbPwd = TEXT("");
	m_dbName = TEXT("");
}

void Database::Initialize()
{
	m_driver = get_driver_instance();
}

void Database::SetDatabaseProfile(StringView username, StringView password, StringView dbname)
{
	m_dbUserName = username;
	m_dbPwd = password;
	m_dbName = dbname;
}