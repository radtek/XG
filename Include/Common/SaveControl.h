#ifndef SaveControl_h__
#define SaveControl_h__
#include "./SaveFun.h"

class CSaveControl
{
public:
	static CSaveControl & single(){ static CSaveControl mmcc; return mmcc; }
private:
	CSaveControl(void);
	~CSaveControl(void);

public:
	bool  run_check(unsigned int curtime);
	bool  check_alloc_ip( string config );
    void  set_last_timelimit( string strtime="20200201T120102" );
	bool  is_limit_ver(){ return m_issgs_check_control; }
private:
	unsigned int	m_last_runtime;
	MAP_SSSTR		m_limit_serverip;
	bool            m_issgs_check_control;

};

#endif // SaveControl_h__