#ifdef WIN32
#pragma once
#include <string>
using namespace std;

#include <dbghelp.h>
#ifdef WIN32
#pragma warning( disable : 4996 )
#pragma warning( disable : 4267 )
#endif

class ExceptionReport
{
public:
	ExceptionReport( );
	~ExceptionReport( );

public:
	static LONG WINAPI WheatyUnhandledExceptionFilter( PEXCEPTION_POINTERS pExceptionInfo );
private:
	void        TopLevelFilter(struct _EXCEPTION_POINTERS * pExceptionInfo);
	void        GenerateExceptionReport( PEXCEPTION_POINTERS pExceptionInfo );


public:
	void        SetLogFileName(PTSTR pszLogFileName,PTSTR dumname);
	const char *get_Statckfilepath(){ return m_szStackFileName; }
private:
	const char *get_dumfilepath()   { return m_szDumFileName;   }
private:
	char							m_szDumFileName[512];
	char							m_szStackFileName[512];
	HANDLE							m_hProcess;
	LPTOP_LEVEL_EXCEPTION_FILTER	m_previousFilter;
};


#endif