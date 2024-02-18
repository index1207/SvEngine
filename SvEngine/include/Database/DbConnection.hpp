#pragma once
#include <sql.h>
#include <sqlext.h>

enum
{
	WVARCHAR_MAX = 4000,
	BINARY_MAX = 8000	
};

class DBConnection
{
public:
	bool Connect(SQLHENV henv, std::wstring_view connectionString);
	void Clear();

	bool Execute(std::wstring_view query);
	bool Fetch();
	int32 GetRowCount();
	void Unbind();
public:
	bool BindParam(int32 paramIdx, bool* value, SQLLEN* idx);
	bool BindParam(int32 paramIdx, float* value, SQLLEN* idx);
	bool BindParam(int32 paramIdx, double* value, SQLLEN* idx);
	bool BindParam(int32 paramIdx, int8* value, SQLLEN* idx);
	bool BindParam(int32 paramIdx, int16* value, SQLLEN* idx);
	bool BindParam(int32 paramIdx, int32* value, SQLLEN* idx);
	bool BindParam(int32 paramIdx, int64* value, SQLLEN* idx);
	bool BindParam(int32 paramIdx, TIMESTAMP_STRUCT* value, SQLLEN* idx);
	bool BindParam(int32 paramIdx, const WCHAR* str, SQLLEN* idx);
	bool BindParam(int32 paramIdx, const BYTE* bin, int32 size, SQLLEN* idx);

	bool BindCol(int32 columnIdx, bool* value, SQLLEN* idx);
	bool BindCol(int32 columnIdx, float* value, SQLLEN* idx);
	bool BindCol(int32 columnIdx, double* value, SQLLEN* idx);
	bool BindCol(int32 columnIdx, int8* value, SQLLEN* idx);
	bool BindCol(int32 columnIdx, int16* value, SQLLEN* idx);
	bool BindCol(int32 columnIdx, int32* value, SQLLEN* idx);
	bool BindCol(int32 columnIdx, int64* value, SQLLEN* idx);
	bool BindCol(int32 columnIdx, TIMESTAMP_STRUCT* value, SQLLEN* idx);
	bool BindCol(int32 columnIdx, WCHAR* buffer, int32 size, SQLLEN* idx);
	bool BindCol(int32 columnIdx, BYTE* buffer, int32 size, SQLLEN* idx);
private:
	bool BindParam(int32 paramIndex, SQLSMALLINT cType, SQLSMALLINT sqlType, SQLULEN len, SQLPOINTER ptr, SQLLEN* index);
	bool BindCol(int32 columnIndex, SQLSMALLINT cType, SQLULEN len, SQLPOINTER value, SQLLEN* index);
	void HandleError(SQLRETURN ret);
private:
	SQLHDBC m_connection = SQL_NULL_HANDLE;
	SQLHSTMT m_statement = SQL_NULL_HANDLE;
};

