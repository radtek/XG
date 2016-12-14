
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

//���ļ��е����ж��嶼��namespace snda��
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

		///��������߳�֮����������
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

		///��ֹ������ֹͣ�����߳�
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

		///��ȡ��ǰ���̸߳���
		size_t GetThreadCount() { return m_vecThreads.size(); }

		///����һ���̶߳���
		void AddThread(CThread* pThread) { if (pThread) m_vecThreads.push_back(pThread); }

		///��ȡһ���̶߳���
		CThread* GetThread(unsigned int nIndex)
		{
			if (nIndex >= m_vecThreads.size())
				return NULL;
			return m_vecThreads[nIndex];
		}

	protected:
		///�Ѵ������̹߳������б�
		std::vector<CThread*> m_vecThreads;
		unsigned int m_lastnotice;
	};

};