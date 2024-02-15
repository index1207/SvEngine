#include "pch.h"
#include "Database/DbConnection.hpp"

DbConnection::DbConnection(SQLHENV env)
{
	if (SQLAllocHandle(SQL_HANDLE_DBC, env, &m_connection) != SQL_SUCCESS)
		throw std::runtime_error("Alloc handle exception");

	if (SQLAllocHandle(SQL_HANDLE_STMT, m_connection, &m_statement) != SQL_SUCCESS)
		throw std::runtime_error("Alloc handle exception");
}

DbConnection::~DbConnection()
{
	Clear();
}

bool DbConnection::Connect(std::string_view connectionString)
{
	char strBuf[MAX_PATH] = "";
	std::copy(connectionString.begin(), connectionString.end(), strBuf);

	char resStr[MAX_PATH] = "";
	SQLSMALLINT resStrLen = 0;

	auto res = SQLDriverConnect(
		m_connection,
		NULL,
		reinterpret_cast<SQLCHAR*>(strBuf),
		_countof(strBuf),
		reinterpret_cast<SQLCHAR*>(resStr),
		_countof(resStr),
		&resStrLen,
		SQL_DRIVER_NOPROMPT
	);

	return res == SQL_SUCCESS || res == SQL_SUCCESS_WITH_INFO;
}

void DbConnection::Clear()
{
	if (m_connection != SQL_NULL_HANDLE)
	{
		SQLFreeHandle(SQL_HANDLE_DBC, m_connection);
		m_connection = SQL_NULL_HANDLE;
	}
	if (m_statement != SQL_NULL_HANDLE)
	{
		SQLFreeHandle(SQL_HANDLE_STMT, m_statement);
		m_statement = SQL_NULL_HANDLE;
	}
}

bool DbConnection::Execute(std::string_view query)
{
	auto res = SQLExecDirect(m_statement, (SQLCHAR*)query.data(), SQL_NTSL);
	if (res == SQL_SUCCESS || res == SQL_SUCCESS_WITH_INFO)
		return true;

	HandleError(res);
	return false;
}

bool DbConnection::Fetch()
{
	auto ret = SQLFetch(m_statement);
	switch (ret)
	{
	case SQL_SUCCESS:
	case SQL_SUCCESS_WITH_INFO:
		return true;
	case SQL_NO_DATA:
		return false;
	case SQL_ERROR:
		HandleError(ret);
	default:
		return true;
	}
}

int32 DbConnection::GetRowCount()
{
	SQLLEN count = 0;
	auto ret = SQLRowCount(m_statement, &count);
	if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
		return static_cast<int32>(count);
	return -1;
}

void DbConnection::Unbind()
{
	SQLFreeStmt(m_statement, SQL_UNBIND);
	SQLFreeStmt(m_statement, SQL_RESET_PARAMS);
	SQLFreeStmt(m_statement, SQL_CLOSE);
}

bool DbConnection::BindParam(SQLUSMALLINT paramIdx, SQLSMALLINT cType, SQLSMALLINT sqlType, SQLULEN len, SQLPOINTER ptr, SQLLEN* idx)
{
	auto ret = SQLBindParameter(m_statement, paramIdx, SQL_PARAM_INPUT, cType, sqlType, len, 0, ptr, 0, idx);
	if (ret != SQL_SUCCESS && SQL_SUCCESS_WITH_INFO)
	{
		HandleError(ret);
		return false;
	}
	return true;
}

bool DbConnection::BindCol(SQLUSMALLINT columnIdx, SQLSMALLINT cType, SQLULEN len, SQLPOINTER value, SQLLEN* idx)
{
	auto ret = SQLBindCol(m_statement, columnIdx, cType, value, len, idx);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
	{
		HandleError(ret);
		return false;
	}
	return true;
}

void DbConnection::HandleError(SQLRETURN ret)
{
	if (ret == SQL_SUCCESS)
		return;

	SQLSMALLINT idx = 1;
	SQLCHAR sqlState[MAX_PATH] = "";
	SQLINTEGER nativeErr = 0;
	SQLCHAR errMsg[MAX_PATH] = "";
	SQLSMALLINT msgLen = 0;
	SQLRETURN errRet;

	while (true)
	{
		errRet = SQLGetDiagRec(
			SQL_HANDLE_STMT,
			m_statement,
			idx,
			sqlState,
			&nativeErr,
			errMsg,
			_countof(errMsg),
			&msgLen
		);
		if (errRet == SQL_NO_DATA)
			break;
		if (errRet != SQL_SUCCESS && errRet != SQL_SUCCESS_WITH_INFO)
			break;

		std::stringstream ss;
		ss << errMsg;
		std::string str;
		ss >> str;

		Console::Log(str);
	}
}