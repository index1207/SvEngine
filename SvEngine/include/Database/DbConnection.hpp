#pragma once

struct MYSQL;

class DBConnection
{
public:
	DBConnection(MYSQL* connection);
	~DBConnection();
public:
	void Execute(std::string_view query);
private:
private:
	MYSQL* m_connection;
};

