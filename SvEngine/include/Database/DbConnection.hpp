#pragma once

class DbConnection
{
public:
	DbConnection(SQLHENV env);
	~DbConnection();
public:
	bool Connect(std::string_view connectionString);
	void Clear();

	bool Execute(std::string_view query);
	bool Fetch();
	
	int32 GetRowCount();
	void Unbind();
public:
	bool BindParam(SQLUSMALLINT paramIdx, SQLSMALLINT cType, SQLSMALLINT sqlType, SQLULEN len, SQLPOINTER ptr, SQLLEN* idx);
	bool BindCol(SQLUSMALLINT columnIdx, SQLSMALLINT cType, SQLULEN len, SQLPOINTER value, SQLLEN* idx);
	void HandleError(SQLRETURN ret);
private:
	SQLHDBC m_connection;
	SQLHSTMT m_statement;
};

