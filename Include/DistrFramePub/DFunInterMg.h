/********************************************************************
	created:	11:11:2016   15:04
	filename: 	G:\AztServer\CxxServer\game_public\DistrFramePub\DFunInterMg.h
	author:		ZhangHongtao  email: 1265008170@qq.com
	
	purpose:	Âß¼­×¢²áµÄ½Ó¿Ú
*********************************************************************/

#ifndef DFunInterMg_h__
#define DFunInterMg_h__
#include "./DFunInterface.h"

class CDsfIfaceMg
{
private:
	CDsfIfaceMg();
	~CDsfIfaceMg();
public:
	static CDsfIfaceMg &Single();

public:
	void            RunTime();
	CXDFIface *		RegIface( CXDFIface *pFace );
	CXDFIface *		GetIface( string strName );
	CXDFIface *		GetIface( zint64 uID );

private:
	MapAFunFace		m_MapName;
	MapBFunFace		m_MapID;
};

#endif // DFunInterMg_h__