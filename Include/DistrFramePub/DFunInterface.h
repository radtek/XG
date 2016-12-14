/********************************************************************
	created:	11:11:2016   15:04
	filename: 	G:\AztServer\CxxServer\game_public\DistrFramePub\DFunInterface.h
	author:		ZhangHongtao  email: 1265008170@qq.com
	
	purpose:	逻辑注册的接口
*********************************************************************/

#ifndef DFunInterface_h__
#define DFunInterface_h__
#include "./DFunParam.h"

class CXDFIface
{
protected:
	CXDFIface();
public:
	virtual ~CXDFIface();

public: 
	virtual void RunTime(){}
	//各种请求接口，自己的接口不需要都实现，只需要实现自己的就可以了，这里要尽量的通用
	virtual	int	DowithReq( const PacketBase *pReqBMsg,PacketBase *pRepBMsg ){ return 9999901; }

public:
	string		get_scriptname(){ return string(m_FunName); }
	string		GetFunName(){ return string(m_FunName); }
	zint64      GetFunID(){ return m_FunID; }

private:
	friend void GfunDFrame_SetFunNameID(CXDFIface *pFun,string strName,zint64 iFunID );
	void		SetFunNameID( string strName,zint64 uID );
private:
	char		m_FunName[64];
	zint64   	m_FunID;
};
typedef map<string,CXDFIface*>MapAFunFace;
typedef map<zint64,CXDFIface*>MapBFunFace;
void    GfunDFrame_RegFun( CXDFIface *pFun );
void    GfunDFrame_SetFunNameID(CXDFIface *pFun,string strName,zint64 iFunID );
////@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
////@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
////@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
#define XHDF_REG_FUNFACE_BYNAME( ClassA,FunfaceName )\
class BOOST_PP_CAT(CDFramHelpAReg_,ClassA)\
{\
public:\
	BOOST_PP_CAT(CDFramHelpAReg_,ClassA)( string strFunName )\
	{\
		static zint64 sc_FunID = sc_xdf_infunid_begin;\
		ClassA *pFun = new ClassA();\
		GfunDFrame_SetFunNameID(pFun,FunfaceName,sc_FunID++ );\
		GfunDFrame_RegFun( pFun );\
	}\
	BOOST_PP_CAT(~CDFramHelpAReg_,ClassA)(){}\
};\
static BOOST_PP_CAT(CDFramHelpAReg_,ClassA) BOOST_PP_CAT(g_,BOOST_PP_CAT(CDFramHelpAReg_,ClassA))(FunfaceName);

////@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
#define XHDF_REG_FUNFACE_BYID( ClassA,MSGID)\
class BOOST_PP_CAT(CDFramHelpBReg_,ClassA)\
{\
public:\
	BOOST_PP_CAT(CDFramHelpBReg_,ClassA)( zint64 iMsgID )\
	{\
		ClassA *pFun = new ClassA();\
		string  strFunName = boost::str( boost::format("XDF_Msg_%u")%iMsgID );\
		GfunDFrame_SetFunNameID(pFun,strFunName,iMsgID );\
		GfunDFrame_RegFun( pFun );\
	}\
	BOOST_PP_CAT(~CDFramHelpBReg_,ClassA)(){}\
};\
static BOOST_PP_CAT(CDFramHelpBReg_,ClassA) BOOST_PP_CAT(g_,BOOST_PP_CAT(CDFramHelpBReg_,ClassA))(MSGID)

#endif // DFunInterface_h__
