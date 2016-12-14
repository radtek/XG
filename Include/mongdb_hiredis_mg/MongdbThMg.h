/********************************************************************
	created:	29:10:2015   19:42
	filename: 	F:\ZtyServer\XFrame\Frame\mongdb_hiredis_mg\MongdbThMg.h
	author:		ZhangHongtao  email: 1265008170@qq.com
	
	purpose:	
*********************************************************************/

#ifndef MongdbThMg_h__
#define MongdbThMg_h__
#include "./MongdbWork.h"

//给低层分配使用
typedef sgs_comment::pool_smallblock< BSONObj,40000,100>PoolMongdbMg;

class MongdbThMg : public CThreadPool
{
public:
	static MongdbThMg * single(){ static MongdbThMg dbsys; return &dbsys; }
private:
	MongdbThMg(void);
	virtual ~MongdbThMg(void);

public:
	int     Init( string configxml );
	void    UnInit();
	int		RunMsg( unsigned int imaxcount );
	bool    IsHaveFault();

public: //更高逻辑的封装
	bool	AddReq_Insert( string dbName,string tblName,BSONObjBuilder &ojBDes,HanderMongCallBack cbFun=0 );
	bool	AddReq_Remove( string dbName,string tblName,BSONObjBuilder &obBWhere );
	bool	AddReq_Update( string dbName,string tblName,BSONObjBuilder &ojBDes,BSONObjBuilder &obBWhere,bool isLog=false );
	bool	AddReq_Select( string dbName,string tblName,BSONObjBuilder &obBWhere,HanderMongCallBack cbFun,
		                   BSONObj *pFieldsToReturn=0,int nToReturn = 0,
						   int nToSkip = 0,int queryOptions = 0 , int batchSize = 0 );
	
	//更新后获取
	bool	AddReq_UpdateSelect( string dbName,string tblName,BSONObjBuilder &ojBDes,BSONObjBuilder &obBWhere,
								 HanderMongCallBack cbFun,BSONObj *pFieldsToReturn=0 );
	//如果不存在插入,如果存在了，就不再执行插入操作
	bool	AddReq_SelectInsert( string dbName,string tblName,BSONObjBuilder &obBWhere,BSONObj *pFieldsToReturn,
		                         HanderMongCallBack cbFun,BSONObjBuilder &ojBDes );

public:
	bool	AddReq( TMongoItem* MgoReq);
	bool	PushRep(TMongoItem* MgoReq);
	
	BSONObj*malloc_cbmsg();
	void    free_cbmsg( BSONObj*pcbi );

private:
	void	Stop();
	bool	empty();
	bool	Set_ConnInfo(string ConfigFile);
	int     CheckMongdbSockIsCon();

private:
	MTS_MgoXList			 m_QueueRep;
	VEC_STR			         m_vecConnInfo; //"dbname;collection;192.168.1.251;50000;username;pwd" 
	unsigned int             m_uIndex;

private:
	PoolMongdbMg	         m_cbmsg_pool;
	boost::recursive_mutex   m_pool_mutex;
};

class AllocatorMgoDBCli
{
private:
	AllocatorMgoDBCli();
public:
	~AllocatorMgoDBCli();
public:
	static AllocatorMgoDBCli&	single();

public:
	MongdbWork*		get_MgoDBcli();//获取当前负载最小的Mongodb client 线程

	MongdbWork*		get_MgoDBcli_byrandom();

private:
	//
};
#endif // sgs_server_MySQLDBSys_h__
