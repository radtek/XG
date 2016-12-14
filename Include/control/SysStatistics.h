#ifndef SysStatistics_h__
#define SysStatistics_h__

#include "../../Include/Common/winflat.h"


const unsigned int sc_invalid_syserrorid = 0;
struct TItemError
{
	unsigned int errorid;
	char         errorkey[32];
	char         errordesc[128];
    unsigned int happencount;
	bool         isprint;
	TItemError()
	{
		isprint     = true;
		happencount = 0;
		memset(errorkey,0,sizeof(errorkey) );
		memset(errordesc,0,sizeof(errordesc) );
		errorid = sc_invalid_syserrorid;
	}
};
typedef map<unsigned int,TItemError> MAPID_SYSERROR;
typedef map<string,     TItemError*> MAPKEY_SYSERROR;

// Dbs using
const string  DbsSt_GoodsYB = "UserBuyGoodsYB";   //ÉÌ³Ç¹ºÂò

class CSysStatistics
{

public:
	static CSysStatistics * single(){ static CSysStatistics _check; return &_check; }

private:
	CSysStatistics();
public:
	~CSysStatistics(void);

public:
	unsigned int      add_new_error( string strkey,string strdesc,bool isprint=true );
	const TItemError *find_error( string strkey );
	const TItemError *find_error( unsigned int errorid );

	void              happen_error( unsigned int errorid, int value=1 );
	void              happen_error( string strerrorkey, int value=1 );
    
	void              print( bool isall = false );
private:
	MAPID_SYSERROR    m_mapid_error;
	MAPKEY_SYSERROR   m_mapkey_error;
	unsigned int      m_errorid;
};


#endif