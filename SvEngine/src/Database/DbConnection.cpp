#include "pch.h"
#include "Database/DBConnection.hpp"

bool DBConnection::Connect(SQLHENV henv, std::wstring_view connectionString)
{
	if (::SQLAllocHandle(SQL_HANDLE_DBC, henv, &m_connection) != SQL_SUCCESS)
		return false;

	WCHAR stringBuffer[MAX_PATH] = { 0 };
	std::copy(connectionString.begin(), connectionString.end(), stringBuffer);

	WCHAR resultString[MAX_PATH] = { 0 };
	SQLSMALLINT resultStringLen = 0;

	SQLRETURN ret = ::SQLDriverConnectW(
		m_connection,
		NULL,
		reinterpret_cast<SQLWCHAR*>(stringBuffer),
		_countof(stringBuffer),
		OUT reinterpret_cast<SQLWCHAR*>(resultString),
		_countof(resultString),
		OUT & resultStringLen,
		SQL_DRIVER_NOPROMPT
	);

	if (::SQLAllocHandle(SQL_HANDLE_STMT, m_connection, &m_statement) != SQL_SUCCESS)
		return false;

	return (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO);
}

void DBConnection::Clear()
{
	if (m_connection != SQL_NULL_HANDLE)
	{
		::SQLFreeHandle(SQL_HANDLE_DBC, m_connection);
		m_connection = SQL_NULL_HANDLE;
	}

	if (m_statement != SQL_NULL_HANDLE)
	{
		::SQLFreeHandle(SQL_HANDLE_STMT, m_statement);
		m_statement = SQL_NULL_HANDLE;
	}
}

bool DBConnection::Execute(std::wstring_view query)
{
	SQLRETURN ret = ::SQLExecDirectW(m_statement, (SQLWCHAR*)query.data(), SQL_NTSL);
	if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
		return true;

	HandleError(ret);
	return false;
}

bool DBConnection::Fetch()
{
	SQLRETURN ret = ::SQLFetch(m_statement);

	switch (ret)
	{
	case SQL_SUCCESS:
	case SQL_SUCCESS_WITH_INFO:
		return true;
	case SQL_NO_DATA:
		return false;
	case SQL_ERROR:
		HandleError(ret);
		return false;
	default:
		return true;
	}
}

int32 DBConnection::GetRowCount()
{
	SQLLEN count = 0;
	SQLRETURN ret = ::SQLRowCount(m_statement, OUT &count);

	if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
		return static_cast<int32>(count);

	return -1;
}

void DBConnection::Unbind()
{
	::SQLFreeStmt(m_statement, SQL_UNBIND);
	::SQLFreeStmt(m_statement, SQL_RESET_PARAMS);
	::SQLFreeStmt(m_statement, SQL_CLOSE);
}

bool DBConnection::BindParam(int32 paramIdx, bool* value, SQLLEN* idx)
{
	return BindParam(paramIdx, SQL_C_TINYINT, SQL_TINYINT, sizeof(bool), value, idx);
}

bool DBConnection::BindParam(int32 paramIdx, float* value, SQLLEN* idx)
{
	return BindParam(paramIdx, SQL_C_FLOAT, SQL_FLOAT, 0, value, idx);
}

bool DBConnection::BindParam(int32 paramIdx, double* value, SQLLEN* idx)
{
	return BindParam(paramIdx, SQL_C_DOUBLE, SQL_DOUBLE, 0, value, idx);
}

bool DBConnection::BindParam(int32 paramIdx, int8* value, SQLLEN* idx)
{
	return BindParam(paramIdx, SQL_C_TINYINT, SQL_TINYINT, sizeof(int8), value, idx);
}

bool DBConnection::BindParam(int32 paramIdx, int16* value, SQLLEN* idx)
{
	return BindParam(paramIdx, SQL_C_SHORT, SQL_SMALLINT, sizeof(int16), value, idx);
}

bool DBConnection::BindParam(int32 paramIdx, int32* value, SQLLEN* idx)
{
	return BindParam(paramIdx, SQL_C_LONG, SQL_INTEGER, sizeof(int32), value, idx);
}

bool DBConnection::BindParam(int32 paramIdx, int64* value, SQLLEN* idx)
{
	return BindParam(paramIdx, SQL_C_SBIGINT, SQL_BIGINT, sizeof(int64), value, idx);
}

bool DBConnection::BindParam(int32 paramIdx, TIMESTAMP_STRUCT* value, SQLLEN* idx)
{
	return BindParam(paramIdx, SQL_C_TIMESTAMP, SQL_TIMESTAMP, sizeof(TIMESTAMP_STRUCT), value, idx);
}

bool DBConnection::BindParam(int32 paramIdx, const WCHAR* str, SQLLEN* idx)
{
	SQLLEN size = (wcslen(str)+1) * 2;
	*idx = SQL_NTSL;

	if (size > WVARCHAR_MAX)
		return BindParam(paramIdx, SQL_C_WCHAR, SQL_WLONGVARCHAR, size, (SQLPOINTER)str, idx);
	return BindParam(paramIdx, SQL_C_WCHAR, SQL_WVARCHAR, size, (SQLPOINTER)str, idx);
}

bool DBConnection::BindParam(int32 paramIdx, const BYTE* bin, int32 size, SQLLEN* idx)
{
	if (bin == nullptr)
	{
		*idx = SQL_NULL_DATA;
		size = 1;
	}
	else
		*idx = size;

	if (size > BINARY_MAX)
		return BindParam(paramIdx, SQL_C_BINARY, SQL_LONGVARBINARY, size, const_cast<BYTE*>(bin), idx);
	return BindParam(paramIdx, SQL_C_BINARY, SQL_VARBINARY, size, const_cast<BYTE*>(bin), idx);
}

bool DBConnection::BindCol(int32 columnIdx, bool* value, SQLLEN* idx)
{
	return BindCol(columnIdx, SQL_C_TINYINT, sizeof(bool), value, idx);
}

bool DBConnection::BindCol(int32 columnIdx, float* value, SQLLEN* idx)
{
	return BindCol(columnIdx, SQL_C_FLOAT, sizeof(float), value, idx);
}

bool DBConnection::BindCol(int32 columnIdx, double* value, SQLLEN* idx)
{
	return BindCol(columnIdx, SQL_C_DOUBLE, sizeof(double), value, idx);
}

bool DBConnection::BindCol(int32 columnIdx, int8* value, SQLLEN* idx)
{
	return BindCol(columnIdx, SQL_C_TINYINT, sizeof(int8), value, idx);
}

bool DBConnection::BindCol(int32 columnIdx, int16* value, SQLLEN* idx)
{
	return BindCol(columnIdx, SQL_C_SHORT, sizeof(int16), value, idx);
}

bool DBConnection::BindCol(int32 columnIdx, int32* value, SQLLEN* idx)
{
	return BindCol(columnIdx, SQL_C_LONG, sizeof(int32), value, idx);
}

bool DBConnection::BindCol(int32 columnIdx, int64* value, SQLLEN* idx)
{
	return BindCol(columnIdx, SQL_C_SBIGINT, sizeof(int64), value, idx);
}

bool DBConnection::BindCol(int32 columnIdx, TIMESTAMP_STRUCT* value, SQLLEN* idx)
{
	return BindCol(columnIdx, SQL_C_TIMESTAMP, sizeof(TIMESTAMP_STRUCT), value, idx);
}

bool DBConnection::BindCol(int32 columnIdx, WCHAR* buffer, int32 size, SQLLEN* idx)
{
	return BindCol(columnIdx, SQL_C_WCHAR, size, buffer, idx);
}

bool DBConnection::BindCol(int32 columnIdx, BYTE* buffer, int32 size, SQLLEN* idx)
{
	return BindCol(columnIdx, SQL_C_BINARY, size, buffer, idx);
}

bool DBConnection::BindParam(int32 paramIndex, SQLSMALLINT cType, SQLSMALLINT sqlType, SQLULEN len, SQLPOINTER ptr, SQLLEN* index)
{
	SQLRETURN ret = ::SQLBindParameter(m_statement, static_cast<SQLUSMALLINT>(paramIndex), SQL_PARAM_INPUT, cType, sqlType, len, 0, ptr, 0, index);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
	{
		HandleError(ret);
		return false;
	}

	return true;
}

bool DBConnection::BindCol(int32 columnIndex, SQLSMALLINT cType, SQLULEN len, SQLPOINTER value, SQLLEN* index)
{
	SQLRETURN ret = ::SQLBindCol(m_statement, static_cast<SQLUSMALLINT>(columnIndex), cType, value, len, index);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
	{
		HandleError(ret);
		return false;
	}

	return true;
}

void DBConnection::HandleError(SQLRETURN ret)
{
	if (ret == SQL_SUCCESS)
		return;

	SQLSMALLINT index = 1;
	SQLWCHAR sqlState[MAX_PATH] = { 0 };
	SQLINTEGER nativeErr = 0;
	SQLWCHAR errMsg[MAX_PATH] = { 0 };
	SQLSMALLINT msgLen = 0;
	SQLRETURN errorRet = 0;

	while (true)
	{
		errorRet = ::SQLGetDiagRecW(
			SQL_HANDLE_STMT,
			m_statement,
			index,
			sqlState,
			OUT &nativeErr,
			errMsg,
			_countof(errMsg),
			OUT &msgLen
		);

		if (errorRet == SQL_NO_DATA)
			break;

		if (errorRet != SQL_SUCCESS && errorRet != SQL_SUCCESS_WITH_INFO)
			break;

		Console::Log(LogDatabase, Error, errMsg);

		index++;
	}
}
