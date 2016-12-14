#ifndef ImpMongdb_h__
#define ImpMongdb_h__

#include "../Common/SaveFun.h"

typedef list<BSONObj*> BSONObjList;
typedef const list<BSONObj*> *pCBSONObjList;

class ImpMongdb
{
public:
	ImpMongdb(string ip, int port);
	~ImpMongdb(void);
private:
	ImpMongdb(void);

public:
	int		 Insert( string dbName, string tblName, BSONObjBuilder &ojBDes);
	int		 Update( string dbName, string tblName, BSONObjBuilder &ojBDes, BSONObjBuilder &obBWhere );
	int		 Remove( string dbName, string tblName, BSONObjBuilder &obBWhere );
	int		 Select( pCBSONObjList &objList, string dbName, string tblName, BSONObjBuilder &obBWhere,
					BSONObj *pFieldsToReturn=0,int nToReturn = 0,
					int nToSkip = 0,int queryOptions = 0 , int batchSize = 0);

	void		 DebugSelf();

private:
	int			  connect();

private:
	string		  strEndpoint;
	bool		  isconnected;

private:
	BSONObjList		SelectResult;

private:
	mongo::DBClientConnection *m_pCon;

};

#endif // tt_redis_h__
