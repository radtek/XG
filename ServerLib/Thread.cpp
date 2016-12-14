//#include "../Include/ServerLib/thread.h"
#include "../Include/ServerLib/Thread.h"
#include "../Include/log/interfacelog.h"
#include "../Include/Common/SaveFun.h"

snda::CThread::CThread(void) 
{
	m_pthread		= 0;
	m_isstart		= false;
	m_bTerminated	= false;

	static unsigned int sc_threadid = 0;
	m_threadid = sc_threadid++;
}

snda::CThread::~CThread(void)
{
}

void snda::CThread::WaitFor(void)
{
	while( m_isstart )
	{
		boost::this_thread::sleep( boost::posix_time::milliseconds(1) );
	}
	boost::this_thread::sleep( boost::posix_time::milliseconds(1) );
}

bool snda::CThread::Start(bool bSuspended)
{
	m_bTerminated = false;
	m_pthread = new boost::thread( boost::bind(&CThread::ImpExecute,this));
	m_isstart = true;
	APPLOG_SYSINFO("[Create thread ...] threadid="<<CSgsPubFun::GetThreadID(m_pthread->get_id())<<FUN_FILE_LINE);
    return true;
}

void snda::CThread::Terminate(void)
{
	//APPLOG_SYSINFO( "[termin thread] id="<<m_threadid<<";bTerminated="<<m_bTerminated<<FUN_FILE_LINE );
	m_bTerminated = true;
}

void snda::CThread::ImpExecute()
{
	//APPLOG_SYSINFO( "[ImpExecute begin] id="<<m_threadid<<";bTerminated="<<m_bTerminated );
	bool iswaiting = false;
	boost::unique_lock<boost::mutex> lock( m_mut );
	while( true )
	{
		if( m_bTerminated )
			break;
		//if( iswaiting )
		//	m_cond.wait( lock );
		this->ExecuteOne( iswaiting );
	}
	m_isstart = false;
	APPLOG_SYSINFO( "[thead over] id="<<m_threadid<<";bTerminated="<<m_bTerminated );
}
