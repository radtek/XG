/****************************************************************************************************
	created:	2010-9-23   10:51
	filename: 	f:\game_server_v3\DBS\net_mysql\MySQLDBSys.h
	author:		Zhanghongtao

	purpose:
		要求mysql操作的这个模块，可以做到性能十分的稳定，并且每一个操作db的逻辑都是可以控制的；
	不会因为一个逻辑的操作而影响到整体的性能。
		这样有一个问题，内存会耗费大些。但也是可以控制的。
*****************************************************************************************************/
#ifndef sgs_server_MySQLDBSys_h__
#define sgs_server_MySQLDBSys_h__

#include "./mysqlworker.h"

typedef boost::function<void(CsqlstringRequest *pRequest)>hander_app_logic;

typedef vector<CSysLog_DBInfo::TSQLKindInfo> VECT_KINDINFO;
class CMySQLDBSystem : public CThreadPool
{
public:
	static CMySQLDBSystem * single(){ static CMySQLDBSystem _dbsys; return &_dbsys; }
public:
	CMySQLDBSystem(void);
    virtual ~CMySQLDBSystem(void);

public://系统接口
	bool   Startup(hander_set_querytype_maxcount checkhander,hander_mysqlcallback mysqlcallback);
	void   Stop();
	int    RunMsg( unsigned int imaxcount );

	bool   AddMysqlConnect( CONNECT_INFO &iteminfo,hander_mysqlcallback mysqlcallback );
public://使用者接口

	//这些操作是和用户相关的，所使用的client也是需要是用useraccount,来关联的
	bool   AddSQLRequest_byaccount( const char			   *useraccount,
									const char			   *pSqlStr,
									unsigned int			uSqlLen,      //sql语句长度
									TMysqlQuest            &parambk);

	//这些操作会根据表名称来选择一个连接，如果你使用的表名称为空，则会随意的选择一个client
	bool   AddSQLRequest_bytblname(	const char			   *pSqlStr,
									unsigned int			uSqlLen,      //sql语句长度
									const char             *tblname,
									TMysqlQuest            &parambk);

	//这些操作会随意的选择一个client
	bool   AddSQLRequest_byrandom(	const char			   *pSqlStr,
									unsigned int			uSqlLen,      //sql语句长度
									TMysqlQuest            &parambk);


	bool   AddSQLRequest_BySqlClient( CMySQLClient			*pmyclient,
									  const char			*pSqlStr,
									  unsigned int			 uSqlLen,
									  TMysqlQuest           &parambk);

	//写数据库日志
	bool   AddSQLRequest_suggest( string tblname,const char *strsql,unsigned int sql_len );

public:
	void                    push_result( CsqlstringRequest *pRequest );

public:
	const CSqlReqPoolMg &	get_reqpoolmg(){ return m_PoolReq; }
    void					DBS_CheckDBConnect();
	unsigned long			dbsys_real_escape_string(const char *account,char *to, const char *from, unsigned long length);
	unsigned long			dbsys_real_escape_stringbb(char *to, const char *from, unsigned long length);

	bool                    empty();
public:
	const VECT_KINDINFO&	SysInfo_mysql();
	unsigned int			get_self_memsize();		//单位M

	unsigned int            get_usingcount( int type );

	void                    SetMysqlServerListInfo(vector<CONNECT_INFO>  &vecConnectInfo);

private:
	void                    CheckMysqlConnect();
    unsigned int            m_lastcheckconnecttime;
private:
	//池管理
	CSqlReqPoolMg          m_PoolReq;
	TQUEUE_SQLREQUEST	   m_SysQueueResult; //处理完成的队列

private://调试信息
	VECT_KINDINFO		   m_mysqlopinfo;
	///服务器连接信息列表
	vector<CONNECT_INFO>   m_vecConnectInfo;
};
#endif // sgs_server_MySQLDBSys_h__
