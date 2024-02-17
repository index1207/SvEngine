#pragma once

class DBConnection;
struct MYSQL;

struct ConnectionData
{
	ConnectionData(std::string_view host, std::string_view user, std::string_view password, std::string_view database, uint32 port)
		: host(host), user(user), password(password), database(database), port(port)
	{}
	std::string host;
	std::string user;
	std::string password;
	std::string database;
	uint32 port;
};

class DBManager
{
public:
	DBManager();
	~DBManager();
public:
	bool Connect(int32 count, ConnectionData data);
	void Clear();
	__forceinline MYSQL* GetHandle() { return m_handle; }
private:
	MYSQL* m_handle;
	ConcurrencyVector<DBConnection*> m_connections;
};

