/****************************************************************************************************
	created:	2010-7-26   13:35
	filename: 	f:\game_server_v3\publib\Common\pool_smallblock.h
	author:		Zhanghongtao
	
	purpose:	提供小块的内存管理
	管理的对象只是简单对象，如果T内部有string等复杂对象，请不要使用
*****************************************************************************************************/
#ifndef sgs_server_pool_smallblock_h__
#define sgs_server_pool_smallblock_h__

//#include "./winflat.h"
#include "./SaveFun.h"
#include "./deathcheck.h"
#include "../log/interfacelog.h"

namespace sgs_comment
{
//#define SGS_LOGIC_RIGHT_CHECK_DBSMEM 1
	class xpool_smallblock
	{
	private:
		typedef list<char*>					LIST_SIMPLEJAR;
		typedef map<unsigned int,char>		MAP_SIMPLEJAR;
		//typedef hash_map<unsigned int,char>	MAP_SIMPLEJAR;
		typedef list<char*>					LIST_CHAR_FROFREE;

		unsigned int m_blocksize;
		unsigned int m_scmax_count;
		unsigned int m_scstep;
	public:
		xpool_smallblock(unsigned int _blocksize,unsigned int _scmax_count,unsigned int _scstep=1000)
		{
			 m_is_may_newmem	= true;
			 m_blocksize		= _blocksize;
			 m_scmax_count		= _scmax_count;
			 m_scstep			= _scstep;
			 m_totalcount		= 0;
		}
		~xpool_smallblock(void)
		{
			UnInit();
		}
		string getloginfo()
		{
			ostringstream strlog;
			strlog<<" [bsize="<<m_blocksize<<",cmax_count="<<m_scmax_count<<",totalcount="<<m_totalcount<<"; FreeCount="<<m_listFree.size()<<" ]";
			return strlog.str();
		}
		char* malloc()
		{
			if( m_listFree.empty() && GetAllCount()<m_scmax_count )
			{
				if( false == AddNewObj( m_scstep ) )
					return 0;
			}

			char* pObj = 0;
			if( !m_listFree.empty())
			{
				pObj = m_listFree.front();
				m_listFree.pop_front();
			}

			if( pObj )
				memset( pObj,0,m_blocksize );
			else{
				APPLOG_ERROR("m_blocksize="<<m_blocksize<<"; m_totalcount="<<m_totalcount<<FUN_FILE_LINE);
			}
			return pObj;
		}
		void free(char* pObj)
		{
			if( pObj )
			{
				#ifdef SGS_LOGIC_RIGHT_CHECK_DBSMEM
					unsigned int upobj = (unsigned int)pObj;
					if( m_listAll.end() == m_listAll.find(upobj) )
					{
						APPLOG_FATAL("m_listAll.size="<<m_listAll.size()<<"; upobj="<<upobj<<FUN_FILE_LINE);
					}
				#endif

				m_listFree.push_front( pObj );
			}
		}
		size_t GetFreeCount(void)
		{
			size_t n = 0;
			n = m_listFree.size();
			return n;
		}
		unsigned int  GetAllCount()
		{
			return m_totalcount;
		}
		long GetSimpleObjMemSize()
		{
			return m_totalcount*m_blocksize;
		}
		void set_stop_allocnewmem(){ m_is_may_newmem=false; }
		void set_may_allocnewmem(){ m_is_may_newmem=true; }
		bool is_may_allocnewmem()const{ return m_is_may_newmem;}

		//先分配一定数目的内存
		unsigned int init_count(unsigned int count)
		{
			while( GetAllCount()<count )
			{
				if( false == AddNewObj( m_scstep ) )
					break;
			}
			return GetAllCount();
		}
	private:
		void UnInit()
		{
			char *pItem = 0;
			for(LIST_CHAR_FROFREE::iterator item=m_list_for_free.begin(); item!=m_list_for_free.end(); item++)
			{
				pItem = *item;
				delete []pItem;
			}
			#ifdef SGS_LOGIC_RIGHT_CHECK_DBSMEM
				m_listAll.clear();
			#endif
			m_listFree.clear();
		}
		bool AddNewObj( unsigned int count )
		{
			//char buffdebug[512]={0};
			//sprintf(buffdebug,"CImpAlloc..AddNewObj:%d,%d",m_blocksize,count );
			//CHelpDeathCheck  myachloadtime(buffdebug,true,1);

			unsigned int pagememM = 0;
			unsigned int pagememK = 0;
			/*unsigned int workmem = */CSgsPubFun::get_process_memory_info( pagememM,pagememK );
			if( pagememK>1024*1024*5 )//5G
			{
				APPLOG_ERROR("[内存已经超过了系统所限制的最大内存] pagememK(M)="<<pagememK<<FUN_FILE_LINE);
				return false;
			}

			char *pTempChar = new (std::nothrow) char[count*m_blocksize];
			if( !pTempChar )
			{
				APPLOG_ERROR("Memory allocation fail: size="<<count*m_blocksize<<FUN_FILE_LINE);
				return false;
			}
			char *pItem = pTempChar;
			m_list_for_free.push_back( (char*)pItem );
			for(unsigned int  i=0; i<count; ++i)
			{
				m_listFree.push_back( pItem );

				//错误检查
				#ifdef SGS_LOGIC_RIGHT_CHECK_DBSMEM
					unsigned int upobj = (unsigned int)pItem;
					m_listAll[upobj] = 't';
				#endif 

				pItem +=m_blocksize;
				m_totalcount++;
			}

			//APPLOG_ERROR("......................");
			return true;
		}

	private:
		LIST_CHAR_FROFREE   m_list_for_free;
		LIST_SIMPLEJAR		m_listFree;
		unsigned int        m_totalcount;
		bool                m_is_may_newmem; //是否可以分配新的内存

#ifdef SGS_LOGIC_RIGHT_CHECK_DBSMEM
		MAP_SIMPLEJAR		m_listAll;
#endif 
	};

	template <typename T,unsigned int scmax_count,unsigned int scstep=1000>
	class pool_smallblock : public xpool_smallblock
	{
	public:
		pool_smallblock(void):xpool_smallblock(sizeof(T),scmax_count,scstep){}
		~pool_smallblock(void){}
		T* malloc()
		{
			T* pObj = (T*)xpool_smallblock::malloc();
			//if( pObj ){new(pObj) T();}
			return pObj;
		}
		void free(T* pObj)
		{
			if( pObj )
			{
				//pObj->~T();
				xpool_smallblock::free( (char*)pObj );
			}
		}
	};
}

namespace AutoTestBase
{
	using namespace sgs_comment;
	class CAT_xpool_smallblock
	{
	public:
		CAT_xpool_smallblock():m_smbloc(64,100,10)
		{
			char *pitem = m_smbloc.malloc();
			m_smbloc.free( pitem );
		}
		~CAT_xpool_smallblock(){}
	private:
		xpool_smallblock  m_smbloc;
	};


	class CAT_pool_smallblock
	{
		struct TStd
		{
			TStd(){}
			~TStd(){}
			int  x;
			int  y;
			int  z;
			char buffer[124];
		};

		typedef pool_smallblock<TStd,100,10 > CCPSmallblock;
	public:
		CAT_pool_smallblock()
		{
			TStd *pitem = m_smbloc.malloc();
			m_smbloc.free( pitem );
		}
		~CAT_pool_smallblock(){}
	private:
		CCPSmallblock  m_smbloc;
	};
}


#endif // sgs_server_pool_smallblock_h__
