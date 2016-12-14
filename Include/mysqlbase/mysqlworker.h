#ifndef sgs_server_mysqlworker_h__
#define sgs_server_mysqlworker_h__

#include "./dbsop_assist.h"

//这个函数工作在mysql的查询线程里面
typedef boost::function<void(CsqlstringRequest *pSqlReq, CMySQLRecordSet &mysqlResSet)>hander_mysqlcallback;

class CMySQLClient:public CThread
{
public:
    CMySQLClient( class CMySQLDBSystem *pSystem,
		          string myname,
				  hander_mysqlcallback mysqlcallback);
    virtual ~CMySQLClient(void);

    bool   ConnectDB(const char *pHostName,int nPort,const char *pDBName,
		             const char *pUserName,const char *pPwd,const char *pConFormat);
    bool   ConnectDB(void);
	bool   CheckDBConnect(bool isexe_reconnect=false);

public:
	void               PushSqlReq( CsqlstringRequest *pSqlReq );

	unsigned long      real_escape_string(char *to, const char *from, unsigned long length);

	bool               empty(){ return m_SysQueueReq.empty(); }
private:
    void               dowith_mysql_record( CsqlstringRequest *pSqlReq, CMySQLRecordSet &mysqlResSet );

private:
    class CMySQLDBSystem   * m_pDBSystem;
    CMySQLConnection *       m_pMySqlConn;

private:
	TQUEUE_SQLREQUEST		 m_SysQueueReq;    //我自己需要处理的请求队列
    string                   m_myname;
	hander_mysqlcallback     m_mysqlcallback;
public:
	CSysLog_DBInfo           m_SysInfo;

protected:
    void   ExecuteOne(bool &iswaiting);
private:
	unsigned int  m_lastupdatetime;
};
#endif // sgs_server_mysqlworker_h__
