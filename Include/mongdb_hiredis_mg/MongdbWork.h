/********************************************************************
	created:	29:10:2015   19:42
	filename: 	F:\ZtyServer\XFrame\Frame\mongdb_hiredis_mg\HiredisThMg.h
	author:		ZhangHongtao  email: 1265008170@qq.com
	
	purpose:	
*********************************************************************/
#ifndef MongdbWork_h__
#define MongdbWork_h__
#include "../Common/SaveFun.h"
#include "../../Include/ServerLib/Thread.h"
#include "../../Include/Common/timeouts_queue.hpp"
#include "../../Include/allocmem/alloc_basemg.h"
using namespace snda;
#include "../../XDepend/mongodbcxx104/include/mongo/client/dbclient.h"

enum emMongoOpType
{
	emMongOp_NoDef	= 0,
	emMongOp_Insert,
	emMongOp_Select,
	emMongOp_Update,
	emMongOp_Remove,
	emMongOp_UpdateSelect,
	emMongOp_SelectInsert,
	emMongOp_Count
};

typedef	boost::function<void(class TMongoItem *pItem)>HanderMongCallBack;
typedef list<BSONObj*>ListBSONObj;
class TMongoItem
{
public:
	TMongoItem();
	~TMongoItem();

public:
	int							set_insertobj( string strDbName, string strCollName, BSONObj *pObjInsert );
	int							set_removeobj( string strDbName, string strCollName, BSONObj *pObjWhere  );
	int							set_selectobj( string strDbName, string strCollName, BSONObj *pObjWhere,BSONObj *pFieldsToReturn=0 );
	int							set_updateobj( string strDbName, string strCollName, BSONObj *pObjWhere,BSONObj *pObjDes,bool isLog );
	int							set_updateselectobj( string strDbName, string strCollName, BSONObj *pObjDes, BSONObj *pObjWhere,BSONObj *pFieldsToReturn);
	int                         set_selectinsertobj(string strDbName, string strCollName, BSONObj *pObjWhere,BSONObj *pFieldsToReturn,BSONObj *pObjInsert );
	void						set_cbfun(HanderMongCallBack cbfun){ m_cbFun=cbfun; }
	const ListBSONObj &         get_select_objs();
	const BSONObj *             get_select_obj();

public:
	int 						get_result();
	string                      loginfo(){ return boost::str( boost::format("[Db.Coll=%s,OpType=%d,index=%u] ")%m_DbCollName%((int)m_OpType)%m_iDebugIndex ); }
	string                      LogSelect();

protected:
	int							CheckParam(string strDbName, string strCollName, BSONObj *pObjInsert);
	void                        AddOneSelectItem( BSONObj *pObj ){ m_selListObj.push_back(pObj); };
	const char*					get_DbCollName();
	void                        set_Result( int v){ m_result=v; }
	bool                        IsLog(){ return m_IsLog; }

public:
	friend class MongdbWork;
	friend class MongdbThMg;
	friend int   infilefun_emMongOp_Select( string db_coll,DBClientConnection *pCon,TMongoItem *pMogoReq );

private:
	char                        m_DbName[32];
	char                        m_DbCollName[64];
	BSONObj                    *m_pWhereObj;      //查询，更新使用
	BSONObj                    *m_pInsertObj;
	BSONObj                    *m_pReturnObj;
	ListBSONObj                 m_selListObj;
	unsigned int                m_iDebugIndex;

	int							m_nToReturn; 
	int							m_nToSkip;
	int							m_queryOptions;
	int							m_batchSize;

	bool                        m_IsLog;
private:
	HanderMongCallBack			m_cbFun;
	emMongoOpType				m_OpType;
	int							m_result;

};

typedef XThreadSaveQueue< TMongoItem*>MTS_MgoXList;

//基于数据库的连接
class MongdbWork:public CThread
{
public:
	MongdbWork( class MongdbThMg *pSystem );
	virtual ~MongdbWork(void);

public:
	int				init(string strConnInfo);
	void			PushMgoReq(TMongoItem *pMgoReq );//接任务

	string			GetDbName(){ return m_dbname; }
	unsigned int	GetTaskSize() { return m_MyQueueReq.size(); }
	int             CheckSockIsCon();
	bool            IsFault(){ return m_MongConOkTime>0; }
private:
	bool			empty();
	void			ExecuteOne(bool &iswaiting);//执行任务
	int 			ImpDowithOne(TMongoItem *pMogoReq);
	void            CloseConnect( string strWhy );

private:
	char                m_dbname[32];
	char                m_strConInfo[128];
	MongdbThMg		   *m_pMongMg;
	DBClientConnection *m_pCon;
	MTS_MgoXList		m_MyQueueReq;    //我自己需要处理的请求队列
	unsigned            m_MongConOkTime; //mongdb和数据库服务器的网络连接成功的时间
};

#endif // sgs_server_mysqlworker_h__
