#ifndef CCBEReqParam_h__
#define CCBEReqParam_h__
/********************************************************************
	created:	7:12:2016   17:27
	filename: 	G:\AztServer\XFrame\Include\ComboxEvent\CCBEReqParam.h
	author:		ZhangHongtao  email: 1265008170@qq.com
	
	purpose:	�����������, ʲôʱ����Ҫ���.
				<1>: ����cf���������ʵ�ʱ����Ҫ����.
				<2>: һ��������Ҫ���ss��ϵ�ʱ��
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
// 	unsigned		m_TcpReqMsgID;		//Tcp�������ϢID
// 	char			m_HttpFunName[64]; //Http����ĺ�������
// };
// typedef map<unsigned,CCBEReqParam*>MapCBEReqPm;

#endif // CCBEReqParam_h__
