
#pragma once
#include "../../Include/Common/winflat.h"
#ifdef WIN32
#pragma warning( push )
#pragma warning( disable : 4244 )
#include <boost/thread.hpp>
#pragma warning( pop ) 
#else
#include <boost/thread.hpp>
#endif

extern unsigned int g_curserver_time;

//本文件中的所有定义都在namespace snda下
namespace snda
{
	const size_t THREAD_INFO_LEN=128;
	class CThread
	{
	public:
		CThread(void);
		virtual ~CThread(void);

		bool			Start(bool bSuspended = false);
		void			Terminate(void);  
		const char *	GetThreadInfo() { return (char *)&m_ThreadInfo;}
		bool			IsStarted(){ return m_isstart; }
		void            WaitFor();
	protected:
		virtual void	SetThreadInfo(){};
		virtual void	ExecuteOne(bool &iswaiting) = 0;
	private:
		void            ImpExecute();
	private:
		char			m_ThreadInfo[128];
		bool            m_isstart;
		boost::thread * m_pthread;

	private:
		boost::mutex               m_mut;
		unsigned int               m_threadid;

	protected:
		volatile bool	m_bTerminated;

	};

	class CThreadPool
	{
	public:
		CThreadPool(void) 
		{
			m_lastnotice=g_curserver_time;
		}
		virtual ~CThreadPool(void)
		{
			if (m_vecThreads.size() > 0)
				ShutDown();
		}

		///在添加了线程之后启动管理
		void Start()
		{
			for (unsigned int i = 0; i < m_vecThreads.size(); i++)
			{
				CThread* pThread = m_vecThreads[i];
				if (!pThread->IsStarted())
					pThread->Start();
			}
		}
		void NoticeRun()
		{
			if( g_curserver_time-m_lastnotice<0 )
				return;
			m_lastnotice=g_curserver_time;
		}

		///终止管理，并停止所有线程
		void ShutDown()
		{
			for (unsigned int i = 0; i < m_vecThreads.size(); i++)
				m_vecThreads[i]->Terminate();
			while(m_vecThreads.size() > 0)
			{
				CThread* pThread = m_vecThreads[0];
				pThread->WaitFor();
				delete pThread;
				m_vecThreads.erase(m_vecThreads.begin());
			}
		}

		///获取当前的线程个数
		size_t GetThreadCount() { return m_vecThreads.size(); }

		///增加一个线程对象
		void AddThread(CThread* pThread) { if (pThread) m_vecThreads.push_back(pThread); }

		///获取一个线程对象
		CThread* GetThread(unsigned int nIndex)
		{
			if (nIndex >= m_vecThreads.size())
				return NULL;
			return m_vecThreads[nIndex];
		}

	protected:
		///已创建的线程管理类列表
		std::vector<CThread*> m_vecThreads;
		unsigned int m_lastnotice;
	};

};