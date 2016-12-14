#ifndef WndConsole_h__
#define WndConsole_h__
#include "./SaveFun.h"

class CWndConsole
{
public:
	CWndConsole(void);
	~CWndConsole(void);
public:
	void	Init();
	void    PrintLog( const char *pBuffer );
private:
#ifdef WIN32
	HANDLE	m_hOutputHandle;
#endif
};

#endif // WndConsole_h__
