/********************************************************************
	created:	23:11:2016   13:39
	filename: 	G:\AztServer\XFrame\Include\pyscript\zhtPyIFaceMg.h
	author:		ZhangHongtao  email: 1265008170@qq.com
	
	purpose:	����ͳһ��ʽ

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
		purpose:	��C++����,�û�ע��һЩC++������python����
	*********************************************************************/
	void			RegCppFun(   string funname,HandCFunAForPy funobj );


public:
	/********************************************************************
		created:	23:11:2016   17:34
		author:		ZhangHongtao  email: 1265008170@qq.com
		purpose:	�ж�python�Ƿ����������ע��
	*********************************************************************/
	bool            IsHavePyFun(string funname);


	/********************************************************************
		created:	23:11:2016   17:34
		author:		ZhangHongtao  email: 1265008170@qq.com
		purpose:	�ж�python�Ƿ��������ע�ᣬ������Ƿ����������
	*********************************************************************/
	bool            IsHavePyFun(string className,string funname);

	/********************************************************************
		created:	23:11:2016   17:34
		author:		ZhangHongtao  email: 1265008170@qq.com
		purpose:	C++����python��������Ȼ�������������ע��
	*********************************************************************/
	string			CallPyAFun( string funName,zint64 iOne,string strOne,string strTwo );

	/********************************************************************
		created:	23:11:2016   17:34
		author:		ZhangHongtao  email: 1265008170@qq.com
		purpose:	C++����pythonһ����ĺ�������Ȼ��������ʵ���ˣ��������������
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
