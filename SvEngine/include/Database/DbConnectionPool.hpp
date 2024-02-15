#pragma once

class DbConnectionPool
{
public:
	DbConnectionPool();
	~DbConnectionPool();
private:
	SQLHENV m_env;

};