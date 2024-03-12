#pragma once

#include "DBConnection.hpp"

template<int32 C> struct FullBits { enum { value = (1 << (C - 1)) | FullBits<C - 1>::value }; };
template<> struct FullBits<1> { enum { value = 1 }; };
template<> struct FullBits<0> { enum { value = 0 }; };

template<int32 paramCount, int32 columnCount>
class Statement
{
public:
	Statement(DBConnection* connection) : m_connection(connection)
	{
		memset(m_paramIdx, 0, sizeof(m_paramIdx));
		memset(m_columnIdx, 0, sizeof(m_columnIdx));
		m_paramFlag = 0;
		m_columnFlag = 0;
		m_connection->Unbind();
	}
	Statement(DBConnection* connection, String sql) : Statement(connection)
	{
		m_query = sql;
	}

	bool Validate()
	{
		return m_paramFlag == FullBits<paramCount>::value && m_columnFlag == FullBits<columnCount>::value;
	}

	bool ExecuteQuery()
	{
		ASSERT_CRASH(Validate());
		return m_connection->Execute(m_query);
	}

	bool Next()
	{
		return m_connection->Fetch();
	}
public:
	template<class T>
	void SetParameter(int32 idx, T& value)
	{
		m_connection->BindParam(idx+1, &value, &m_paramIdx[idx]);
		m_paramFlag |= 1LL << idx;
	}
	void SetParameter(int32 idx, const WCHAR* value)
	{
		m_connection->BindParam(idx+1, value, &m_paramIdx[idx]);
		m_paramFlag |= 1LL << idx;
	}
	template<class T, int32 N>
	void SetParameter(int32 idx, T(&value)[N])
	{
		m_connection->BindParam(idx+1, reinterpret_cast<const BYTE*>(value), sizeof(T) * N, &m_paramIdx[idx]);
		m_paramFlag |= 1LL << idx;
	}
	template<class T>
	void SetParameter(int32 idx, T* value, int32 length)
	{
		m_connection->BindParam(idx+1, reinterpret_cast<const BYTE*>(value), sizeof(T) * length, &m_paramIdx[idx]);
		m_paramFlag |= 1LL << idx;
	}

	template<class T>
	void Bind(int32 idx, T& value) {
		m_connection->BindCol(idx+1, &value, &m_columnIdx[idx]);
		m_columnFlag |= 1LL << idx;
	}
	template<int32 N>
	void Bind(int32 idx, WCHAR(&value)[N])
	{
		m_connection->BindCol(idx+1, value, N*2, &m_columnIdx[idx]);
		m_columnFlag |= 1LL << idx;
	}
	void Bind(int32 idx, WCHAR* value, int32 size)
	{
		m_connection->BindCol(idx+1, value, size*2, &m_columnIdx[idx]);
		m_columnFlag |= 1LL << idx;
	}
	template<class T, int32 N>
	void Bind(int32 idx, T(&value)[N])
	{
		m_connection->BindCol(idx+1, value, sizeof(T) * N, &m_columnIdx[idx]);
		m_columnFlag |= 1LL << idx;
	}
private:
	String m_query;
	DBConnection* m_connection;
	SQLLEN m_paramIdx[paramCount > 0 ? paramCount : 1];
	SQLLEN m_columnIdx[columnCount > 0 ? columnCount : 1];
	uint64 m_paramFlag;
	uint64 m_columnFlag;
};
