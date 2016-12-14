/***********************************************************************************************************
	created:	15:11:2016   15:29
	filename: 	G:\AztServer\XFrame\Include\ComboxEvent\ComboxEvent.h
	author:		ZhangHongtao  email: 1265008170@qq.com
	
	purpose:	保存一次请求，多次返回；或者是一次请求，需要异步处理的网络请求等事件。
				使用场景
					1: 多次数据库操作
					2: 多次网络操作

************************************************************************************************************/

#ifndef ComboxEvent_h__
#define ComboxEvent_h__
#include "./ComboxEvtPub.h"

enum EmComEvtOverResult
{
	EComEvtORet_Ok = 0,
	EComEvtORet_TimeOut,	//超时了
	EComEvtORet_HappenErr,	//发送了错误
};
typedef boost::function<void ( unsigned uID )>hander_comboxevt_begin;
typedef boost::function<void ( unsigned uID,EmComEvtOverResult result )>hander_comboxevt_over;
class CComboxEvt : public XObjMark
{
public:
	enum EmXMark
	{
		EmXM_IsTcp = 1<<0,	//是否是tcp
	};
public:
	CComboxEvt();
	virtual ~CComboxEvt();

public:	
	void					OnTimer();

	void                    SetClientConID( unsigned v ){ m_ClientConid=v; }
	unsigned                GetClientConID(){ return m_ClientConid; }

	unsigned				GetMyID(){ return m_myID; }

	bool                    IsTcp(){ return this->IsXMark(EmXM_IsTcp); }
	bool                    IsRepOver();

	void					SetLiveTm( unsigned v ){ m_LiveMaxTm=v; }
	unsigned				GetLiveTm(){ return m_LiveMaxTm; }
	bool                    IsTimeOut();

	bool					IsHaveErr(){ return strlen(m_ErrInfo)>0; }
	void					SetErr( string strInfo );
	void					SetErr( string strInfo,int iErrPos );
	string					GetErr(){ return m_ErrInfo; }

	//这个函数可以重复使用
	bool					SetNextFun( hander_comboxevt_begin hBegin );

	//参数的保存
	unsigned                GetNewParamID();
	int						AddParam(  CCEvtParamBase *pEvtPm );
	CCEvtParamBase *		GetParam( unsigned uParamID );
	int                     GetParamCount(){ return m_parames.size(); }
	CCEvtParamBase *		GetParamByIndex( int index );

	int						AddParam( const PacketBase *pBMsg,unsigned uParamID );
	CCEvtPaNetMsg *			GetPmNetMsg( unsigned uParamID );

	//保存请求参数
	void					SetReqPm( CCEvtParamBase *pBMsg );
	CCEvtParamBase *        GetReqPm(){ return m_pReqPm; }
public:	
	virtual	int				OnEventMsg( const PacketBase *pBMsg );
	virtual	int				OnEventMsg( Json::Value *pJsMsg );

protected:
	bool					IsOver(){ return m_IsOver; }
	void                    SetOver(){ m_IsOver=true; }

private:
	unsigned				m_ClientConid;

private:
	unsigned				m_myID;
	bool					m_IsOver;
	unsigned				m_CreateTm;		//创建事件
	unsigned				m_LiveMaxTm;	//生命周期，多长时间后超时
	char					m_ErrInfo[128];	//错误信息

	CCEvtParamBase         *m_pReqPm;       //启动这个事件的，请求参数,tcp，http   
	ListCEvtPaBase			m_parames;		//数据备份池，当然也可以不用
};
typedef map<unsigned,CComboxEvt*>MapComboxEvt;

#endif // ComboxEvent_h__
