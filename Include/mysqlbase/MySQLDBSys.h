/****************************************************************************************************
	created:	2010-9-23   10:51
	filename: 	f:\game_server_v3\DBS\net_mysql\MySQLDBSys.h
	author:		Zhanghongtao

	purpose:
		Ҫ��mysql���������ģ�飬������������ʮ�ֵ��ȶ�������ÿһ������db���߼����ǿ��Կ��Ƶģ�
	������Ϊһ���߼��Ĳ�����Ӱ�쵽��������ܡ�
		������һ�����⣬�ڴ��ķѴ�Щ����Ҳ�ǿ��Կ��Ƶġ�
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

public://ϵͳ�ӿ�
	bool   Startup(hander_set_querytype_maxcount checkhander,hander_mysqlcallback mysqlcallback);
	void   Stop();
	int    RunMsg( unsigned int imaxcount );

	bool   AddMysqlConnect( CONNECT_INFO &iteminfo,hander_mysqlcallback mysqlcallback );
public://ʹ���߽ӿ�

	//��Щ�����Ǻ��û���صģ���ʹ�õ�clientҲ����Ҫ����useraccount,��������
	bool   AddSQLRequest_byaccount( const char			   *useraccount,
									const char			   *pSqlStr,
									unsigned int			uSqlLen,      //sql��䳤��
									TMysqlQuest            &parambk);

	//��Щ��������ݱ�������ѡ��һ�����ӣ������ʹ�õı�����Ϊ�գ���������ѡ��һ��client
	bool   AddSQLRequest_bytblname(	const char			   *pSqlStr,
									unsigned int			uSqlLen,      //sql��䳤��
									const char             *tblname,
									TMysqlQuest            &parambk);

	//��Щ�����������ѡ��һ��client
	bool   AddSQLRequest_byrandom(	const char			   *pSqlStr,
									unsigned int			uSqlLen,      //sql��䳤��
									TMysqlQuest            &parambk);


	bool   AddSQLRequest_BySqlClient( CMySQLClient			*pmyclient,
									  const char			*pSqlStr,
									  unsigned int			 uSqlLen,
									  TMysqlQuest           &parambk);

	//д���ݿ���־
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
	unsigned int			get_self_memsize();		//��λM

	unsigned int            get_usingcount( int type );

	void                    SetMysqlServerListInfo(vector<CONNECT_INFO>  &vecConnectInfo);

private:
	void                    CheckMysqlConnect();
    unsigned int            m_lastcheckconnecttime;
private:
	//�ع���
	CSqlReqPoolMg          m_PoolReq;
	TQUEUE_SQLREQUEST	   m_SysQueueResult; //������ɵĶ���

private://������Ϣ
	VECT_KINDINFO		   m_mysqlopinfo;
	///������������Ϣ�б�
	vector<CONNECT_INFO>   m_vecConnectInfo;
};
#endif // sgs_server_MySQLDBSys_h__
