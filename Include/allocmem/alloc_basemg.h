/****************************************************************************************************
	created:	2010-9-3   15:15
	filename: 	f:\game_server_v3\GS\gameLogic\core\memalloc\alloc_basemg.h
	author:		Zhanghongtao
	
	purpose:	
*****************************************************************************************************/
#ifndef sgs_server_alloc_basemg_h__
#define sgs_server_alloc_basemg_h__
#include "./imp_alloc.h"

struct TAcBlock
{
	unsigned int  uBkLen;
	char         *pBuff;
	TAcBlock()
	{
		uBkLen = 0;
		pBuff  = 0;
	}
};

template<class AMG,bool isneed_counter = true>
class CAllocBaseMg
{
protected:
	CAllocBaseMg(string counter_name="")
	{
		m_pcounter_allocmg    = 0;
		m_curusing_blockcount = 0;
		if(isneed_counter && counter_name.size()>0 )
		{
			string strdesc = "app alloc blockcountused for ";
			strdesc += counter_name;
			m_pcounter_allocmg = INiceNetCounter::CreateCounter("appframe::allocmg",counter_name.c_str(), strdesc.c_str() );
		}
		m_imp = CImpAlloc::single();
	}

	virtual ~CAllocBaseMg()
	{
	}
public:
	static AMG *single()
	{ 
		static AMG _allocmg; return &_allocmg;
	}

    void UnInit()
	{
		if( isneed_counter )
		{
			DELETE_COUNTER(m_pcounter_allocmg);
		}
	}

	void RunCounter()
	{
		if( isneed_counter )
			m_pcounter_allocmg->UpdateReplace( m_curusing_blockcount );
	}

public:
	bool is_valid(char *pobj) //是否是有效的内存指针
	{
		return m_imp->is_valid(pobj);
	}

protected:
	char * _malloc( unsigned int size  )
	{
		if( size<=0 )
			return 0;

		char *pret = m_imp->malloc(size);
		if( pret ) ++m_curusing_blockcount;
		return pret;
	}
	bool _free( char *pobj,int marktype )
	{
		if( !pobj )
			return false;

		if( m_imp->free(pobj,marktype) )
		{
			--m_curusing_blockcount;
			return true;
		}
		return false;
	}

private:
	unsigned int         m_curusing_blockcount;
	INiceNetCounter *    m_pcounter_allocmg;
	class CImpAlloc *    m_imp;
protected:
};

//通用的分配池
class CSgsAllocXBlockMg : public CAllocBaseMg<CSgsAllocXBlockMg>
{
protected:
	friend class CAllocBaseMg<CSgsAllocXBlockMg>;
	CSgsAllocXBlockMg():CAllocBaseMg<CSgsAllocXBlockMg>("sgsalloc"){}
	virtual ~CSgsAllocXBlockMg(){}

public:
	char * malloc( unsigned int ma_size )
	{
		return _malloc( ma_size );
	}

	bool free( char *pobj, int marktype=21800 )
	{
		return _free( pobj,marktype );
	}

	//////////////////////////////////////////////////////////////////////////
	int FillTAcBlock( TAcBlock &acbk, const char *pxml )
	{
		acbk.uBkLen = 0;
		if( !pxml )
			return 0;
		if( acbk.uBkLen>0 )
		{
			APPLOG_ERROR("[logic error] "<<FUN_FILE_LINE );
		}
		acbk.uBkLen = strlen( pxml );
		acbk.pBuff  = this->malloc( acbk.uBkLen );
		memcpy( acbk.pBuff,pxml,strlen(pxml) );
		return 0;
	}

	int FreeTAcBlock( TAcBlock &acbk )
	{
		if( acbk.pBuff )
		{
			this->free( acbk.pBuff,324530 );
			acbk.pBuff  = 0;
			acbk.uBkLen = 0;
		}
		return 0;
	}

	//////////////////////////////////////////////////////////////////////////
	template<class XList,class XItem>
	void FreeList(XList &xlist)
	{
		XItem *pItem    = 0;
		for(typename XList::iterator it = xlist.begin(); it!=xlist.end(); it++ )
		{
			pItem = *it;
			this->free( (char*)pItem );
		}
		xlist.clear();
	}

	template<class XList,class XItem>
	void DeleteList(XList &xlist)
	{
		XItem *pItem    = 0;
		for(typename XList::iterator it = xlist.begin(); it!=xlist.end(); it++ )
		{
			pItem = *it;
			this->delete_xx<XItem>( pItem );
		}
		xlist.clear();
	}

	template<class XMap,class XItem>
	void FreeXMap(XMap &xmap)
	{
		XItem *pItem    = 0;
		for(typename XMap::iterator it = xmap.begin(); it!=xmap.end(); it++ )
		{
			pItem = it->second;
			this->free( (char*)pItem );
		}
		xmap.clear();
	}
	template<class XMap,class XItem>
	void DeleteXMap(XMap &xmap)
	{
		XItem *pItem    = 0;
		for(typename XMap::iterator it = xmap.begin(); it!=xmap.end(); it++ )
		{
			pItem = it->second;
			this->delete_xx<XItem>( pItem );
		}
		xmap.clear();
	}

	template<class A>
	A *new_0xx()
	{
		A* pA = (A*)malloc( sizeof(A) );
		if( pA )
		{ new(pA)A(); }
		return pA;
	}
	template<class A,typename PARAM0 >
	A *new_1xx( PARAM0 param0 )
	{
		A* pA = (A*)malloc( sizeof(A) );
		if( pA )
		{ new(pA)A(param0); }
		return pA;
	}

	template<class A,typename PARAM0,typename PARAM1>
	A *new_2xx(PARAM0 param0,PARAM1 param1)
	{
		A* pA = (A*)malloc( sizeof(A) );
		if( pA )
		{ new(pA)A(param0,param1); }
		return pA;
	}

	template<class A,typename PARAM0,typename PARAM1,typename PARAM2>
	A *new_3xx(PARAM0 param0,PARAM1 param1,PARAM2 param2)
	{
		A* pA = (A*)malloc( sizeof(A) );
		if( pA )
		{ new(pA)A(param0,param1,param2); }
		return pA;
	}

	template<class A,typename PARAM0,typename PARAM1,typename PARAM2,typename PARAM3>
	A *new_4xx(PARAM0 param0,PARAM1 param1,PARAM2 param2,PARAM3 param3)
	{
		A* pA = (A*)malloc( sizeof(A) );
		if( pA )
		{ new(pA)A(param0,param1,param2,param3); }
		return pA;
	}

	template<class A,typename PARAM0,typename PARAM1,typename PARAM2,typename PARAM3,typename PARAM4>
	A *new_5xx(PARAM0 param0,PARAM1 param1,PARAM2 param2,PARAM3 param3,PARAM4 param4)
	{
		A* pA = (A*)malloc( sizeof(A) );
		if( pA )
		{ new(pA)A(param0,param1,param2,param3,param4); }
		return pA;
	}

	template<class A,typename PARAM0,typename PARAM1,typename PARAM2,typename PARAM3,typename PARAM4, typename PARAM5>
	A *new_6xx(PARAM0 param0,PARAM1 param1,PARAM2 param2,PARAM3 param3,PARAM4 param4, PARAM5 param5)
	{
		A* pA = (A*)malloc( sizeof(A) );
		if( pA )
		{ new(pA)A(param0,param1,param2,param3,param4,param5); }
		return pA;
	}

	template<class A>
	bool delete_xx( A* pA, int marktype=21900 )
	{
		if( pA && is_valid( (char*)pA ) )
		{
			try
			{
				pA->~A();
			}
			catch (...)
			{
				APPLOG_ERROR("[A] 析构异常! pA="<<(long)pA<<",class="<<typeid(A).name()<<FUN_FILE_LINE );
			}
		}
		return free( (char*)pA,marktype );
	}
	template<class A>
	void delete_xx_bb( A* pA, int marktype=21900 )
	{
		delete_xx<A>(pA,marktype);
	}

};


//判断释放是一个有效的内存快,使用我们自己的内存管理
extern bool if_valid_mempoint( char *pobj );
#endif // sgs_server_alloc_basemg_h__
