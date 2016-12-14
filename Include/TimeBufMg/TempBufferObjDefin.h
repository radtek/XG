#ifndef TempBufferObjDefin_h__
#define TempBufferObjDefin_h__
#include "../../Include/Common/winflat.h"
#include "boost/function.hpp"

enum emTempItemypeOne
{
	emTempTypeOne_Error  = 0,
	emTempTypeOne_BufMsg,
	emTempTypeOne_Obj,
};
struct TTempItemType
{
	unsigned int	typeone;
};

//////////////////////////////////////////////
class  CTempObjItem
{
protected:
	CTempObjItem(){ m_calpos=0; m_usermark=0; }
public:
	virtual ~CTempObjItem(){}
public:
	unsigned int  m_calpos;
	unsigned int  m_usermark;
};

//////////////////////////////////////////////////////////////////////////
struct TItemObjItem
{
public:
	const CTempObjItem *m_pObj;
};
struct TItemMsgItem
{
public:
	char          *m_pbuff;
	unsigned int   m_bufflen;
	unsigned int   m_usermark;     //用户自己设置的信息
};

//////////////////////////////////////////////////////////////////////////
struct TTempItemHead
{
	unsigned int		begintime;
	unsigned int		keeptimelen;
	TTempItemType		type;

	union uInTData
	{
		TItemMsgItem   BufMsg;
		TItemObjItem   Obj;
	}indata;
};
typedef map<unsigned int,TTempItemHead*> MAPIDD_VOID;
typedef map<string,      TTempItemHead*> MAPKEY_VOID;

typedef boost::function< bool(unsigned int, TItemMsgItem *)> HandMsgBuf_TimeoutKeyUint;
typedef boost::function< bool(string      , TItemMsgItem *)> HandMsgBuf_TimeoutKeyStr;
typedef boost::function< bool(unsigned int, TItemObjItem *)> HandObjBuf_TimeoutKeyUint;
typedef boost::function< bool(string      , TItemObjItem *)> HandObjBuf_TimeoutKeyStr;

#endif //TempBufferObjDefin_h__