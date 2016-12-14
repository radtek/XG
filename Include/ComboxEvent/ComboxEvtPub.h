/********************************************************************
	created:	15:11:2016   16:50
	filename: 	G:\AztServer\XFrame\Include\ComboxEvent\ComboxEvtPub.h
	author:		ZhangHongtao  email: 1265008170@qq.com
	
	purpose:	
*********************************************************************/

#ifndef ComboxEvtPub_h__
#define ComboxEvtPub_h__
#include "../json/json.h"
#include "../Common/XDirtyMark.h"
#include "../allocmem/alloc_basemg.h"
#include "../xframe_protocol/xframe_client_pro.h"

class CCEvtParamBase : public XObjMark
{
protected:
	CCEvtParamBase();

public:
	virtual ~CCEvtParamBase();

public:
	unsigned			GetPmID(){ return m_ParamID; }
	bool				IsDowith(){ return this->IsXMark(1); }
	void				SetDowith(){ this->SetXMark(1); }
	int					SetDataBuf( const char *pData, int uDataLen );
	bool				IsRecRepMsg(){ return m_DaBuf.datalen>0; }

	//tcp «Î«Û”√
	const PacketBase *	GetPacketBase();
	int              	SetPacketBase(const PacketBase *pBMsg);

	//http ”√
	const TBufferItem * GetDaBuf(){ return &m_DaBuf; }

protected:
	friend class CComboxEvt;
	unsigned	 m_ParamID;
	TBufferItem  m_DaBuf;
};
typedef list<CCEvtParamBase*>ListCEvtPaBase;


class CCEvtPaNetMsg : public CCEvtParamBase
{
public:
	CCEvtPaNetMsg();
	virtual ~CCEvtPaNetMsg();
public:

};
#endif // ComboxEvtPub_h__
