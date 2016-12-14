/********************************************************************
	created:	29:10:2015   19:42
	filename: 	F:\ZtyServer\XFrame\Frame\mongdb_hiredis_mg\HiredisThMg.h
	author:		ZhangHongtao  email: 1265008170@qq.com
	
	purpose:	
*********************************************************************/
#ifndef HiredisThMg_h__
#define HiredisThMg_h__
#include "./HiredisWork.h"
#include "../Common/SaveFun.h"

enum emHiredisOpType{
	OpNoDef=0,
	OpGET,
	OpSET,
	OpTTL,
	OpDEL,
	//read, write, read write,write read
};

typedef	boost::function<void(class THiredisItem *pResult)> callback_handleResult;
typedef	boost::function<void( TBufferItem &ReadBuf,int iErrID)>cbfun_redis_read;
typedef	boost::function<void()>cbfun_redis_write;
typedef	boost::function<void( unsigned int expireSecond,int iErrID)>cbfun_redis_ttl;
class THiredisItem
{
public:
	THiredisItem( emHiredisOpType OpType,callback_handleResult cbfun ){ m_OpType=OpType; m_callback=cbfun; expire_time=0; }
	~THiredisItem();

public:
	int 						if_ok_result(){ return result; }
	void						Redis_response();//call back
	emHiredisOpType				get_HiredisOpType();
	int							get_CustmOpType();
	void						set_Result(int Result);

public:
	char						chkey[128]; //key
	unsigned int				expire_time;//expire time
	TBufferItem					WriteData;  //value
	TBufferItem					ReadData;   //result

private:
	callback_handleResult		m_callback;
	emHiredisOpType             m_OpType;
	int							result;
};

typedef XThreadSaveQueue<THiredisItem*>XTSQ_THiredisIT;
class HiredisThMg : public CThreadPool
{
private:
	HiredisThMg() { m_TaskList_MaxCount = 1000;};

public:
	struct TaskStatus 
	{
		size_t	reqs_szie;
		size_t  reps_szie;
	};

public:
	static HiredisThMg * single(){ static HiredisThMg hir_dbsys; return &hir_dbsys; }

public:
	int                 Init( string configxml );
	void                UnInit();
	int					RunMsg( unsigned int imaxcount );

	bool				SetRedisConnectInfo(string RedisConfigFile);

	//request相关操作
	int 				PushRedisReq( THiredisItem *pRequest );
	THiredisItem*		get_req( bool &isgetok );		
	bool				empty() { return  m_Reqs.empty()?true:false; }

	//结果相关
	void				push_result( THiredisItem * pRequest );

	//获取任务当前状态
	TaskStatus			get_CurrentTaskStatus();

//封装一些更高级的接口,方便使用
public:
	bool				GetData(string strkey, int iMaxLen,cbfun_redis_read cbfun);
	bool				SetData(string strkey, const char* pBinData,int dataLen,int expire,cbfun_redis_write cbfun);
	bool				RemoveData( string strkey );
	int                 GetTtl( string strkey,cbfun_redis_ttl cbfun );

private:
	bool				Set_ConnInfo(string configxml);
	void				CheckAndReconnect();
	unsigned int        m_lastchecktime;

private:
	XTSQ_THiredisIT		m_Reqs;					//To do list, multi thread access
	XTSQ_THiredisIT		m_Reps;					//Done, main thread handle result
	unsigned int		m_TaskList_MaxCount;

	VEC_STR	    m_vecConnectInfo;//"redisname;192.168.1.7;21301;"
};
#endif 
