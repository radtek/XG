#ifdef WIN32

#ifndef get_app_stackinfo_win_h__
#define get_app_stackinfo_win_h__
#include "./winflat.h"
#include <dbghelp.h>

class CGetAppCurStackInfoWin
{
	struct TCallBackData
	{
		STACKFRAME64    sf;
		HANDLE			hProcess;
	};

private:
	CGetAppCurStackInfoWin( );
	~CGetAppCurStackInfoWin( );
public:
	static CGetAppCurStackInfoWin *single();

public:
	int                  dowith_exception( struct _EXCEPTION_POINTERS *ep,const char *plogfilepath);
	bool                 isload_debugfile(){ return m_isloadsys; }
private:
	void		         WriteStackDetails( PCONTEXT pContext,HANDLE hProcess,bool bWriteVariables,bool iswritefile,ostringstream &strloginfo );

private:
	bool         m_isloadsys;
	HANDLE       m_hProcess;
	unsigned int m_randomindex;

};


#endif // get_app_stackinfo_win_h__

#endif // WIN32
