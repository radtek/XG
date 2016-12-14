#ifndef HttpSs_h__
#define HttpSs_h__
#include "../Common/SaveFun.h"
#include "../../Include/xframe_protocol/xframe_server_pro.h"

class CHttpReq
{
private:
	enum EmHttpReqType
	{
		EHttpReq_NoDef = 0,
		EHttpReq_Get,
		EHttpReq_Post,
	};

	struct TBaseInfo
	{
		EmHttpReqType   reqType;
		char            Interface[128];	//接口
		unsigned		Content_Length;	//post数据的长度
		char			Host[32];
	};
public:
	static CHttpReq &Single();

protected:
	CHttpReq(void);
	virtual ~CHttpReq(void);

public:
	int				InitData( const PacketBase *pHMsg );
	int				AnalysisPosDataByStr();

	bool			IsPost(){ return m_BaseInfo.reqType==EHttpReq_Post; }
	bool			IsGet(){  return m_BaseInfo.reqType==EHttpReq_Get; }
	string			GetParam( string strkey );

private:
	void			ReSet();
	int  			DoWithHttpHead_method( VEC_STR &vHead );
	int				TwoFun_GetParam( VEC_STR &vHead );
	int				TwoFun_PostParam( VEC_STR &vHead );

private:
	TBaseInfo       m_BaseInfo;
	TBufferItem		m_PosData;
	MAP_XMLKEY      m_MapStrParam;
};


#endif // HttpSs_h__
