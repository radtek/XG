/********************************************************************
	created:	28:5:2010   11:19
	filename: 	f:\SGS_2\Server\MySqlDBSrv\Public\SimpleObjJar.h
	author:		Zhanghongtao
	
	purpose:	一个简单的池
*********************************************************************/
#ifndef __SIMPLEOBJJAR_ZHT
#define __SIMPLEOBJJAR_ZHT

#include "./winflat.h"
//#include "./AutoLock.h"
#include "../../Include/log/interfacelog.h"

namespace sgs_comment
{
	template <typename T>
    class CSimpleObjJar
	{
	private:
		typedef list<T*> LIST_SIMPLEJAR;
	public:
		CSimpleObjJar(void)
		{
		}
		~CSimpleObjJar(void)
		{
			UnInit();
		}
		bool Init(unsigned int  dwCount)
		{
			return AddNewObj( dwCount )>0;
		}
		bool Increase( unsigned int count )
		{
			return AddNewObj( count )>0;
		}
		void UnInit()
		{
			T *pItem = 0;
			while( !m_listAll.empty() )
			{
				pItem = (T*)(*m_listAll.begin());
				char *pt = static_cast<char *>(((void*)pItem));
				delete []pt;
				m_listAll.pop_front();
			}

			m_listAll.clear();
			m_listFree.clear();
		}
		T* FetchObj(void)
		{
			T* pObj = 0;
			if(!m_listFree.empty())
			{
				pObj = m_listFree.front();
				new(pObj) T();
				m_listFree.pop_front();
			}
			return pObj;
		}
		void ReleaseObj(T* pObj)
		{
			if( !pObj )
			{
				APPLOG_ERROR(FUN_FILE_LINE);
				return;
			}

			pObj->~T();
			m_listFree.push_front( pObj );
		}
		size_t GetFreeCount(void)
		{
			size_t n = 0;
			n = m_listFree.size();
			return n;
		}
        int  GetAllCount()
		{

			return m_listAll.size();
		}
		long GetSimpleObjMemSize()
		{

			return m_listAll.size()*sizeof(T);
		}
	private:
		unsigned int AddNewObj( unsigned int count )
		{
			T *pItem = 0;
			for(unsigned int  i=1; i<=count; ++i)
			{
				//pItem   = (T*)((void*)(new (std::nothrow) char[sizeof(T)]));
				pItem   = (T*)((void*)(new char[sizeof(T)]));
				if( !pItem )
				{
					//APPLOG_ERROR("Allocate mem fail:size="<<sizeof(T)<<";classname="<<typeid(T).name()<<FUN_FILE_LINE);
					return i;
				}	
				//pItem = new T;
				m_listFree.push_back( pItem );
				m_listAll.push_back( pItem );

#ifdef SGS_AUTO_TEST
				if( this->GetAllCount()>10000 && (0==this->GetAllCount()%2000) )
				{
					APPLOG_WARNING("WARNING_ZHT:totalcount="<<this->GetAllCount()<<";classname="<<typeid(T).name()<<FUN_FILE_LINE);
				}
#endif // SGS_AUTO_TEST
			}
			return count;
		}

	private:
		LIST_SIMPLEJAR		m_listFree;
		LIST_SIMPLEJAR		m_listAll;
	};
}
#endif
