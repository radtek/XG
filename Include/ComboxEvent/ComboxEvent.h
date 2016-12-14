/***********************************************************************************************************
	created:	15:11:2016   15:29
	filename: 	G:\AztServer\XFrame\Include\ComboxEvent\ComboxEvent.h
	author:		ZhangHongtao  email: 1265008170@qq.com
	
	purpose:	����һ�����󣬶�η��أ�������һ��������Ҫ�첽���������������¼���
				ʹ�ó���
					1: ������ݿ����
					2: ����������

************************************************************************************************************/

#ifndef ComboxEvent_h__
#define ComboxEvent_h__
#include "./ComboxEvtPub.h"

enum EmComEvtOverResult
{
	EComEvtORet_Ok = 0,
	EComEvtORet_TimeOut,	//��ʱ��
	EComEvtORet_HappenErr,	//�����˴���
};
typedef boost::function<void ( unsigned uID )>hander_comboxevt_begin;
typedef boost::function<void ( unsigned uID,EmComEvtOverResult result )>hander_comboxevt_over;
class CComboxEvt : public XObjMark
{
public:
	enum EmXMark
	{
		EmXM_IsTcp = 1<<0,	//�Ƿ���tcp
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

	//������������ظ�ʹ��
	bool					SetNextFun( hander_comboxevt_begin hBegin );

	//�����ı���
	unsigned                GetNewParamID();
	int						AddParam(  CCEvtParamBase *pEvtPm );
	CCEvtParamBase *		GetParam( unsigned uParamID );
	int                     GetParamCount(){ return m_parames.size(); }
	CCEvtParamBase *		GetParamByIndex( int index );

	int						AddParam( const PacketBase *pBMsg,unsigned uParamID );
	CCEvtPaNetMsg *			GetPmNetMsg( unsigned uParamID );

	//�����������
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
	unsigned				m_CreateTm;		//�����¼�
	unsigned				m_LiveMaxTm;	//�������ڣ��೤ʱ���ʱ
	char					m_ErrInfo[128];	//������Ϣ

	CCEvtParamBase         *m_pReqPm;       //��������¼��ģ��������,tcp��http   
	ListCEvtPaBase			m_parames;		//���ݱ��ݳأ���ȻҲ���Բ���
};
typedef map<unsigned,CComboxEvt*>MapComboxEvt;

#endif // ComboxEvent_h__
