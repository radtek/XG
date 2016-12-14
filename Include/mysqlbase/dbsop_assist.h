#ifndef sgs_server_dbsop_assist_h__
#define sgs_server_dbsop_assist_h__

#include "../../Include/ServerLib/MySQLDB.h"
#include "../../Include/ServerLib/Thread.h"
#include "../../Include/Common/timeouts_queue.hpp"
using namespace snda;

#include "../allocmem/alloc_basemg.h"

//ϵͳ��ȵȱ����
#define  SYSTEM_SERVERDB "systemserver"
#define  NETLOG_SERVERDB "netlog"

const unsigned int qtCheckMySqlConnect = 1;

/********************************************************************
	created:	2010-9-23   10:56 author: Zhanghongtao
	purpose:	������Ϣ
*********************************************************************/
typedef struct tagDBConnectInfo
{    
	char m_hostName[24];		  //DB����������
	int  m_hostPort;		      //DB���������Ӷ˿� 
	char m_dbName[20];            //DB����
	char m_userName[20];          //����DB�û���
	char m_password[30];          //����DB�û�����
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
	bool          mysql_isbin;              //�������mysql�����Ƿ��ж���������
	int           sqlType;                  //����

	hander_mysql_callback  callback;

private:
	friend class CMySQLDBSystem;
	friend class CsqlstringRequest;
	unsigned int  query_record_bufferlen;
	char        * query_record_buffer;      //��ѯ�Ľ�������صĵط�������ڴ�Ҫ���������߼��ɷ���ã�
};

/********************************************************************
	created:	31:3:2011   20:44
	author:		zhanghongtao
	purpose:	�������������Ҫ���ڴ���Ҫ�����߼��������ͳ�ʼ��!
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

public: //����
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
		APPLOG_ERROR("[�������ô���]!");
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
	int           m_sqlType;        //����
	bool          m_mysql_isbin;    //�������mysql�����Ƿ��ж���������
	unsigned int  m_sqlbufferlen;   //sql��仺����
	char        * m_sqlbuffer;      //sql���

private:
	unsigned int  m_query_record_bufferlen;
	char        * m_query_record_buffer;    //��ѯ�Ľ��

private:
	hander_mysql_callback  m_callback;

private: //mysql������ʾ
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
	MAP_REQUEST	        m_self;    //��������
	int                 m_metype;
};

//////////////////////////////////////////////////////////////////////////
//����false��ʾ�����Ѿ������
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
			����DB��������Ϣ
*********************************************************************/
class CSysLog_DBInfo
{
public:
	static const unsigned int sc_max_qtType  = 50;
	struct TSQLKindInfo
	{
		unsigned int 	m_uTotal;              //����ִ�е�����
		unsigned int	m_uFail;               //����ִ��ʧ�ܵ�����
		unsigned int	m_uExcept;             //����ִ���쳣������
		double          m_maxtime;             //ִ��һ�ε����ʱ�䣬����
		double	        m_mintime;             //��Сʱ��
		double			m_avetime;             //ƽ��ʱ��
        double          m_uOKTotalTime;        //ִ�гɹ�����ʱ�䣬Ҳ����������ƽ��ʱ��

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
	purpose:	��Ҫhash�ı��������ṩͳһ�Ľӿڹ���
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
	void					add_hash(	string tblname,unsigned int iconid );  //���һ����
	void					set_conid_client(	unsigned int iconid,class CMySQLClient *pc );//��ӹ����߳�
	class CMySQLClient *	get_client( string tblname=SYSTEM_SERVERDB );        //�����Ǳ���,����ǿգ������
	class CMySQLClient *	get_client_byaccount( string account ); //�������û��˺�

private:
	//ϵͳ��ѯ��Ҫ������ID,�Ժ�Ҳ����������ķ�ʽ
	MAP_HASH_CON     		m_hash_table;   //table-->conid; �� to �߳�
	MAP_CONID_CLIENT        m_map_conid_to_client;
	unsigned int            m_rand_client;
};

#endif // sgs_server_dbsop_assist_h__
