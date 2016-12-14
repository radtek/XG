/********************************************************************
	created:	15:11:2016   15:29
	filename: 	G:\AztServer\XFrame\Include\ComboxEvent\ComboxEvtMg.h
	author:		ZhangHongtao  email: 1265008170@qq.com
	
	purpose:	
*********************************************************************/
#ifndef ComboxEvtMg_h__
#define ComboxEvtMg_h__
#include "./ComboxEvent.h"
#include "./CCBEReqParam.h"

typedef boost::function< void( CComboxEvt *pEvt ) >HandDsfComboxEvtTimeout;
class CComboxEvtMg
{
public:
	CComboxEvtMg();
	~CComboxEvtMg();

public:
	void			RunTime();

	CComboxEvt*		AddCEvt( int iLiveTm );
	CComboxEvt*		GetCEvt( unsigned uEID );
	void			DelCEvt( unsigned uEID );
	void            SetTimeoutHand( HandDsfComboxEvtTimeout v ){ m_handTOut=v; }

private:
	MapComboxEvt			m_mapCEvt;
	HandDsfComboxEvtTimeout m_handTOut;
};


#endif // ComboxEvent_h__
