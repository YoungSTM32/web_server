#ifndef _CONNECTION_POOL
#define _CONNECTION_POOL

#include <mysql/mysql.h>
#include <iostream>
#include <pthread.h>
#include <list>
#include <string>
#include <string.h>
#include <error.h>
#include <stdio.h>
#include "../locker/locker.h"

using namespace std;

class connection_pool{

	public:
		MYSQL* GetConnection();
		bool ReleaseConnection(MYSQL *conn);
		int GetFreeCon();
		void DestoryPool();

		static connection_pool* GetInstance();
		void init(string url,string User,string Passwd,string DataBaseName,int port,int MaxConn,int close_log);

	private:
		connection_pool();
		~connection_pool();

		int m_MaxConn;
		int m_CurConn;
		int m_FreeConn;
		locker lock;
		list<Mysql*> connList;
		sem reserve;
		
	public:
		string m_url;
		string m_port;
		string m_user;
		string m_passwd;
		string m_databaseName;
		int m_close_log;

};

class connectionRAII{

	public:
		connectionRAII(MYSQL** con, connection_pool * connPool);
		~connectionRAII();

	private:
		MYSQL* conRAII;
		connection_pool* poolRAII;

};

#endif

