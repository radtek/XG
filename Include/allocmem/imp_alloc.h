/****************************************************************************************************
	created:	2010-9-4   13:35
	filename: 	f:\game_server_v3\gs\gamelogic\core\memalloc\imp_alloc.h
	author:		Zhanghongtao
	
	purpose:	分配策略，小于128,的整体分配，
	大于128的单个分配
*****************************************************************************************************/
#ifndef sgs_server_imp_alloc_h__
#define sgs_server_imp_alloc_h__

#include "../../Include/Common/stdhead.h"
#include "../../Include/Common/pool_smallblock.h"
#include <boost/function.hpp>

//是否可以分配新的内存
bool  is_allocmem_byapp();

struct gspool_alloc_item 
{
	unsigned int							blocksize;
	class sgs_comment::xpool_smallblock *	pool;  
};

class CImpAlloc
{
	typedef boost::function<char *()>      hander_malloc;
	typedef boost::function<void (char *)> hander_free;

	//static const unsigned int   sc_max_blockmem = 1024*4;
	static const unsigned int   sc_max_blockmem = 1024*16;

private:
	//           size,
	typedef map<unsigned int,gspool_alloc_item> MAP_POOLITEM;

	//这个量会很大
	//typedef map<unsigned int,class sgs_comment::xpool_smallblock *> MAP_POINTTOPOOL;
	typedef SgsStl::hash_map<zint64,class sgs_comment::xpool_smallblock *> MAP_POINTTOPOOL;

protected:
	CImpAlloc();
public:
	~CImpAlloc();
public:
	static CImpAlloc *single();

	void		 init( vector<unsigned int> &vblocksize );
	void		 uninit();
public:
	char *		 malloc( unsigned int size );
	bool		 free( char *pobj, int marktype=0 );
	bool         is_valid(char *pobj);

	void         log_meminfo();
public:
	//unsigned int get_total_mem_size(); //unit k
	//unsigned int get_total_blockcount();
	unsigned int get_total_blockcount_used(){ return m_map_free.size(); }
    
	//设置停止分配新的内存快
	//void           set_stop_allocnewmem();
private:
	class sgs_comment::xpool_smallblock * add_new(unsigned int blocksize,unsigned int step);
    class sgs_comment::xpool_smallblock * find(unsigned int blocksize);
private:
	unsigned int    m_standard_max_size;
	MAP_POOLITEM    m_list_pool;
	MAP_POINTTOPOOL m_map_free;
	bool            m_is_may_allocnewmem;
};


#endif // sgs_server_imp_alloc_h__