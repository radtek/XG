/********************************************************************
	created:	23:11:2016   13:39
	filename: 	G:\AztServer\XFrame\Include\pyscript\zhtPyIFaceMg.h
	author:		ZhangHongtao  email: 1265008170@qq.com
	
	purpose:	调用统一格式

	python call c++: fun( strFunName,zint64,strPma,strPmb)
	c++ call python: fun( FunName,zint64,strPma,strPmb );
	c++ call python: fun( classname,strFunName,zint64,strPma,strPmb );
*********************************************************************/
#ifndef zhtPyIFaceMg_h__
#define zhtPyIFaceMg_h__
#include "../../Include/Common/SaveFun.h"

#ifdef XGAME_ZHT_PYTHON
class CZhtPyIFaceMg
{
private:
	CZhtPyIFaceMg();
public:
	~CZhtPyIFaceMg();

public:
	static CZhtPyIFaceMg & single(){ static CZhtPyIFaceMg mg; return mg; }

public:
	int  			Init( string pyRoot, string strPyDir );
	void			UnInit();

public:
	/********************************************************************
		created:	23:11:2016   17:34
		author:		ZhangHongtao  email: 1265008170@qq.com
		purpose:	给C++调用,用户注册一些C++函数给python调用
	*********************************************************************/
	void			RegCppFun(   string funname,HandCFunAForPy funobj );


public:
	/********************************************************************
		created:	23:11:2016   17:34
		author:		ZhangHongtao  email: 1265008170@qq.com
		purpose:	判断python是否有这个函数注册
	*********************************************************************/
	bool            IsHavePyFun(string funname);


	/********************************************************************
		created:	23:11:2016   17:34
		author:		ZhangHongtao  email: 1265008170@qq.com
		purpose:	判断python是否有这个类注册，这个类是否有这个函数
	*********************************************************************/
	bool            IsHavePyFun(string className,string funname);

	/********************************************************************
		created:	23:11:2016   17:34
		author:		ZhangHongtao  email: 1265008170@qq.com
		purpose:	C++调用python函数，当然这个函数必须先注册
	*********************************************************************/
	string			CallPyAFun( string funName,zint64 iOne,string strOne,string strTwo );

	/********************************************************************
		created:	23:11:2016   17:34
		author:		ZhangHongtao  email: 1265008170@qq.com
		purpose:	C++调用python一个类的函数，当然这个类必须实现了，并且有这个函数
	*********************************************************************/
	string          CallPyBFun( string className,string funName,zint64 iOne,string strOne,string strTwo );

private:
	bool			ReloadScript( string strFile );
	bool			ReloadAll();


private:
	class CZhtImpPyIFaceMg *m_pImp;
};

extern void export_GsSysInfo_py();

#endif // XGAME_ZHT_PYTHON
#endif // csLogicPyVm_h__
