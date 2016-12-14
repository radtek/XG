#ifndef sgs_server_dbsop_assist_h__
#define sgs_server_dbsop_assist_h__

#include "../../Include/ServerLib/MySQLDB.h"
#include "../../Include/ServerLib/Thread.h"
#include "../../Include/Common/timeouts_queue.hpp"
using namespace snda;

#include "../allocmem/alloc_basemg.h"

//系统表等等表操作
#define  SYSTEM_SERVERDB "systemserver"
#define  NETLOG_SERVERDB "netlog"

const unsigned int qtCheckMySqlConnect = 1;

/********************************************************************
	created:	2010-9-23   10:56 author: Zhanghongtao
	purpose:	连接信息
*********************************************************************/
typedef struct tagDBConnectInfo
{    
	char m_hostName[24];		  //DB服务器名称
	int  m_hostPort;		      //DB服务器连接端口 
	char m_dbName[20];            //DB名称
	char m_userName[20];          //访问DB用户名
	char m_password[30];          //访问DB用户密码
	char m_ConFormat[30];         //utf8

	tagDBConnectInfo(const tagDBConnectInfo& dbInfo)
	{
		memcpy(m_hostName, dbInfo.m_hostName, sizeof(m_hostName));
		m_hostPort = dbInfo.m_hostPort;
		memcpy(m_dbName,    dbInfo.m_dbName,    sizeof(m_dbName));
		memcpy(m_userName,  dbInfo.m_userName,  sizeof(m_userName));
		memcpy(m_password,  dbInfo.m_password,  sizeof(m_password));
		memcpy(m_ConFormat, dbInfo.m_ConFormat, sizeof(m_ConFormat));
	}

	tagDBConnectInfo():m_hostPort(0)
	{
		memset(m_hostName, 0, sizeof(m_hostName));
		memset(m_dbName,   0, sizeof(m_dbName));
		memset(m_userName, 0, sizeof(m_userName));
		memset(m_password, 0, sizeof(m_password));
		memset(m_ConFormat, 0, sizeof(m_ConFormat));
	}

}CONNECT_INFO,*PCONNECT_INFO;

//DbopQueryType
static const int sc_max_querytype = 200;

typedef boost::function< void(class CsqlstringRequest * pResSet) >hander_mysql_callback;
typedef boost::function<void(string straccount)>callback_getaccountbynickname;

struct TMysqlQuest
{
public:
	TMysqlQuest(int type,bool isbin)
	{
		mysql_isbin            = isbin;
		sqlType                = type;
		query_record_bufferlen = 0;
		query_record_buffer    = 0;
	}

	template<class A> A *create_TMysqlQuest()
	{
		A* pA = (A*)CSgsAllocXBlockMg::single()->malloc( sizeof(A) );
		if( pA )
		{
			query_record_bufferlen = sizeof(A);
			query_record_buffer    = (char*)pA;
		}
		return pA;
	}

public:
	bool          mysql_isbin;              //你请求的mysql操作是否有二进制数据
	int           sqlType;                  //类型

	hander_mysql_callback  callback;

private:
	friend class CMySQLDBSystem;
	friend class CsqlstringRequest;
	unsigned int  query_record_bufferlen;
	char        * query_record_buffer;      //查询的结果，返回的地方，这个内存要求你在主逻辑成分配好，
};

/********************************************************************
	created:	31:3:2011   20:44
	author:		zhanghongtao
	purpose:	这个对象所有需要的内存需要在主逻辑里面分配和初始化!
*********************************************************************/
class CsqlstringRequest
{
public:
	static const    unsigned short sc_SqlResult_ok     = 0;
	static const    unsigned short sc_SqlResult_except = 1;
	static const    unsigned short sc_SqlResult_unknow = 2;

public:
	CsqlstringRequest();
	~CsqlstringRequest();

public: //请求
	bool          mysql_request( char *pmysql,unsigned int mysql_len,TMysqlQuest &param );
    void          mysql_response();

public:
	template<class A> A *get_xx_a()
	{
		if( m_query_record_buffer && (sizeof(A)==m_query_record_bufferlen) )
		{
			A* pA = (A*)m_query_record_buffer;
			return pA;
		}
		APPLOG_ERROR("[参数设置错误]!");
		return 0;
	}

public:
	bool          if_ok_result(){ return 0==strlen(m_errorinfo);}

public:
	int           get_query_type(){ return m_sqlType; }

public:
	bool          if_bin_mysqlop(){ return m_mysql_isbin; }
	const char *  get_mysql_command(){ return m_sqlbuffer;}
	unsigned int  get_mysql_command_len(){ return m_sqlbufferlen; }

	void          set_mysql_errorinfo( const char *perrinfo, unsigned int errorid );
	const char *  get_mysql_errorinfo(){ return m_errorinfo; }
private:
	int           m_sqlType;        //类型
	bool          m_mysql_isbin;    //你请求的mysql操作是否有二进制数据
	unsigned int  m_sqlbufferlen;   //sql语句缓冲区
	char        * m_sqlbuffer;      //sql语句

private:
	unsigned int  m_query_record_bufferlen;
	char        * m_query_record_buffer;    //查询的结果

private:
	hander_mysql_callback  m_callback;

private: //mysql错误提示
    unsigned int  m_mysqlerror_code;
	char          m_errorinfo[128];

};

typedef map<CsqlstringRequest*,char>MAP_REQUEST;


struct CRequestPool
{
public:
	CRequestPool();
	~CRequestPool();
public:
	void                InitMeType( unsigned int typess ){ m_metype=typess; }
	CsqlstringRequest *	FetchObj();
	bool				ReleaseObj( CsqlstringRequest *Obj );
	unsigned int        get_all_size();

private:
	MAP_REQUEST	        m_self;    //所有桌子
	int                 m_metype;
};

//////////////////////////////////////////////////////////////////////////
//返回false表示队列已经最大了
typedef boost::function<bool(int queytype,unsigned int poolhavesize)>hander_set_querytype_maxcount;
class CSqlReqPoolMg
{
public:
	bool				Init(hander_set_querytype_maxcount hander );
	void                Uninit();

	CsqlstringRequest *	FetchObj(   int type );
	bool				ReleaseObj( CsqlstringRequest *Obj );

	unsigned int        get_type_usingcount( int type );

private:
	hander_set_querytype_maxcount	m_hander_check;
	CRequestPool					m_poolmg[sc_max_querytype];
	MAP_REQUEST						m_all;
};

//request sql
typedef XThreadSaveQueue<CsqlstringRequest *> TQUEUE_SQLREQUEST;


/********************************************************************
	created:	2010-9-23   10:54 author: Zhanghongtao
	purpose:	
			生成DB的性能信息
*********************************************************************/
class CSysLog_DBInfo
{
public:
	static const unsigned int sc_max_qtType  = 50;
	struct TSQLKindInfo
	{
		unsigned int 	m_uTotal;              //请求执行的总数
		unsigned int	m_uFail;               //请求执行失败的总数
		unsigned int	m_uExcept;             //请求执行异常的总数
		double          m_maxtime;             //执行一次的最大时间，毫秒
		double	        m_mintime;             //最小时间
		double			m_avetime;             //平均时间
        double          m_uOKTotalTime;        //执行成功的总时间，也好用来技术平均时间

		TSQLKindInfo()
		{
			m_uTotal  = 0;
			m_uFail   = 0;
			m_uExcept = 0;
			m_maxtime = 0;
			m_mintime = 0XFFFFFFFF;
			m_avetime = 0;
			m_uOKTotalTime = 0;
		}
	};
private:
	TSQLKindInfo     m_SQKKind[	sc_max_qtType ];

public:
	TSQLKindInfo *Find( unsigned int type )
	{
		if( type>=sc_max_qtType )
			return 0;
		return &m_SQKKind[type];
	}
};
/****************************************************************************************************
	author:		Zhanghongtao	
	purpose:	需要hash的表都在这里提供统一的接口管理
*****************************************************************************************************/
const string tblname_youka = "youka_gift";
class CHashInfo
{
	typedef map<string,unsigned int>  MAP_HASH_CON;
	typedef map<unsigned int,class CMySQLClient*> MAP_CONID_CLIENT;
private:
	CHashInfo();
public:
	~CHashInfo();

public:
	static CHashInfo &      single();
	static unsigned int		get_hash_index( string v );

public:
	void					add_hash(	string tblname,unsigned int iconid );  //添加一个表
	void					set_conid_client(	unsigned int iconid,class CMySQLClient *pc );//添加工作线程
	class CMySQLClient *	get_client( string tblname=SYSTEM_SERVERDB );        //参数是表名,如果是空，则随机
	class CMySQLClient *	get_client_byaccount( string account ); //参数是用户账号

private:
	//系统查询需要的连接ID,以后也可能是数组的方式
	MAP_HASH_CON     		m_hash_table;   //table-->conid; 表 to 线程
	MAP_CONID_CLIENT        m_map_conid_to_client;
	unsigned int            m_rand_client;
};

#endif // sgs_server_dbsop_assist_h__
