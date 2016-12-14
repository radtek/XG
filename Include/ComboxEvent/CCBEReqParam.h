#ifndef CCBEReqParam_h__
#define CCBEReqParam_h__
/********************************************************************
	created:	7:12:2016   17:27
	filename: 	G:\AztServer\XFrame\Include\ComboxEvent\CCBEReqParam.h
	author:		ZhangHongtao  email: 1265008170@qq.com
	
	purpose:	保存请求参数, 什么时候需要这个.
				<1>: 请求到cf服务器策率的时候需要缓存.
				<2>: 一个请求，需要多个ss配合的时候
*********************************************************************/
#include "./ComboxEvtPub.h"

// class CCBEReqParam : public XObjMark
// {
// public:
// 	CCBEReqParam();
// 	virtual ~CCBEReqParam();
// 
// public:
// 	bool			IsTcp(){ return this->IsXMark(1); }
// 	unsigned		GetID(){ return m_ID; }
// 	void			SaveReq( const PacketBase *pMsg );
// 	void			SaveReq( IHttpReq *pReq );
// 
// 	unsigned        GetTcpMsgID(){ return m_TcpReqMsgID; }
// 	string          GetHttpFunName(){ return string(m_HttpFunName); }
// 
// private:
// 	unsigned		m_ID;
// 	TBufferItem		m_BufReq;
// 
// 	unsigned		m_TcpReqMsgID;		//Tcp请求的消息ID
// 	char			m_HttpFunName[64]; //Http请求的函数名称
// };
// typedef map<unsigned,CCBEReqParam*>MapCBEReqPm;

#endif // CCBEReqParam_h__
