#include "mysql_con_pool.h"

MYSQL* connection_pool::GetConnection(){

    MYSQL* con=NULL;
    if(0==ConnList.size()){
        return NULL;
    }
    reserve.wait();
    lock.lock;
    con=connList.front();
    connList.pop_front();

    --m_FreeConn;
    ++m_CurConn;

    lock,unlock();
    return con;

}

bool connection_pool::ReleaseConnection(MYSQL* conn){
    if(NULL==conn){
        return false;
    }
    lock.lock();
    connList.push_back(con);
    ++m_FreeConn;
    --m_CurConn;

    lock.unlock();

    reserve.post();
    return true;

}

int connection_pool::GetFreeCon(){
    return m_FreeConn;//get the number of mysql connection.
}

void connection_pool::Destroy(){

    lock.lock();

    if(connList.size()>0){

        list<MYSQL*>::iterator it;
        for(it=connList.begin();it!=connList.end();it++){
            MYSQL* con=*it;
            mysql_close(con);
        }
        m_CurConn=0;
        M_FreeConn=0;
        connList.clear();
    }

    lock.unlock();

}

static connection_pool* connection_pool::GetInstance(){

    static connection_pool connPool;//construction function is going on.
    return &connPool;

}

void connection_pool::init(string url,string User,string Passwd,string DataBaseName,int port,int MaxConn,int close_log){
    m_url=url;
    m_port=Port;
    m_User=User;
    m_passwd=Passwd;
    m_databaseName=""DataBaseName;
    m_close_log=close_log;

    for(int i=0;i<MaxConn;i++){
        MYSQL* con=NULL;
        con=mysql_init(con);
        if(con=NULL){
            LOG_ERROR("Mysql Error");
            exit(1);
        }
        con=mysql_real_connect(con,url.c_str(),User,c_str(),Passwd.c_str(),DataBaseName.c_str(),Port,NULL,0);
        if(con==NULL){

            LOG_ERROR("Mysql Error");
            exit(1);

        }
    connList.push_back(con);
    ++m_FreeConn;
    }
    reserve=sem(m_FreeConn);//show the number of mysql connection.
    m_MaxConn=M_FreeConn;

}



connection_pool::connection_pool(){

    m_CurConn=0;
    m_FreeConn=0;

}
connection_pool::~connection_pool(){

    DestoryPool();

}


connectionRAII::connectionRAII(MYSQL** SQL,connection_pool* connPool){
    *SQL=connPool->GetConnection();
    conRAII=*SQL;
    poolRAII=connPool;
}

connectionRAII::~connectionRAII(){

    poolRAII->releaseConnection(conRAII);

}

