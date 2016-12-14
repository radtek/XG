#ifndef mongdbface_h__
#define mongdbface_h__
#include "../Common/SaveFun.h"

typedef const list<BSONObj*> *pCBSONObjList;

class CFaceMongdb
{
public:
	CFaceMongdb(string ip, int port);
	~CFaceMongdb(void);
private:
	CFaceMongdb(void);


public://synchronization interface
	int			Insert( string dbName, string tblName, BSONObjBuilder &obBWhere);
	int			Update( string dbName, string tblName, BSONObjBuilder &ojBDes, BSONObjBuilder &obBWhere);
	int			Remove( string dbName, string tblName, BSONObjBuilder &obBWhere );
	int         Select( pCBSONObjList &objList,string dbName, string tblName, BSONObjBuilder &obBWhere,
							BSONObj *pFieldsToReturn=0,int nToReturn = 0,
							int nToSkip = 0,int queryOptions = 0 , int batchSize = 0);


	void		  DebugSelf();



private:
	class ImpMongdb *m_pImp;
};

#endif // tt_redis_h__
