/**********************************************************************************************
	created:	2010-7-7   10:18
	filename: 	f:\game_server_v3\publib\Common\timeouts_queue.hpp
	author:		Zhanghongtao

	purpose:
		    下面的一组类，是为了在系统存在隐患的地方进行排队处理，保证整个服务器系统在在用户最多，
		或者系统网络最差，等等恶劣的环境下，可以安全稳定的运行。
			当然提供这些机制是要有一定的用户体验牺牲的。会让用户等待的时间比较系统良好的时候多些。
		所有这些系统的参数设置很重要，需要进行严格的估算和测试。
***********************************************************************************************/


#ifndef __SGS_COMMENT_TIMEOUTSQ__
#define __SGS_COMMENT_TIMEOUTSQ__
#include "./winflat.h"
#include <boost/timer.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/static_assert.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/pool/object_pool.hpp>
#include <boost/pool/pool.hpp>
#include <boost/thread/recursive_mutex.hpp>

#include "./pool_smallblock.h"
#include "./SaveFun.h"
#include "./stdhead.h"

using namespace boost;
using namespace std;
using namespace SgsStl;

extern unsigned int g_server_relativetime;
extern unsigned int g_curserver_time;
/********************************************************************
created:	2010-9-1   15:22 author: Zhanghongtao
purpose:	提供不可重入机制，如果一个容器需要控制不可以重入，请从我
			派生
*********************************************************************/
template <typename key,typename IMPMAP>
class CImpNotReenter
{
	static const char sc_inchar  = 'Y';
	static const char sc_outchar = 'N';
public:
	CImpNotReenter()
	{
	}
	virtual ~CImpNotReenter(){}

public:
	void del( key k)
	{
		typename IMPMAP::iterator it = m_map.find( k );
		if( m_map.end() != it )
		{
			m_map[k] = sc_outchar;
			return;
		}
		//APPLOG_ERROR(FUN_FILE_LINE);
	}
	bool if_in(key k)
	{
		typename IMPMAP::iterator it = m_map.find( k );
		if( m_map.end() == it )
			return false;
		if( sc_inchar == it->second )
			return true;
		return false;
	}
protected:
	void add( key k )
	{
		m_map[k] = sc_inchar;
	}
	void true_del( key k)
	{
		m_map.erase(k);
	}
private:
	IMPMAP  m_map;
};

/***************************************************************************************************************
[name]:  提供结构体的不可重入队列
****************************************************************************************************************/
template <typename A,typename key,typename MYMAP=map<key,char> >
class InterfaceXNotReenterQ : public CImpNotReenter<key,MYMAP >
{
protected:
	//typedef queue<A*>				IN_QUEUE;
	typedef list<A*>				IN_QUEUE;
	typedef sgs_comment::pool_smallblock<A,100000>	MY_POOL;
public:
	InterfaceXNotReenterQ()
	{
		//BOOST_MPL_ASSERT( (boost::is_class<A>) );
		//BOOST_MPL_ASSERT_NOT( (boost::is_pointer<A>) );
	}
	virtual ~InterfaceXNotReenterQ(){}

public:
	bool push( A obj,bool isfront=false/*是否插入到队列的头*/ )
	{
		if( if_in( obj() ) )
			return true;

		A *ppn = m_pool.malloc();
		if( !ppn )
		{
			APPLOG_ERROR("alloc fail:totalcount="<<m_q.size()<<";classname="<<typeid(A).name()<<"; key="<<obj()<<FUN_FILE_LINE);
			return false;
		}
		new( ppn ) A(obj);

		add( obj() );
		if(isfront)
			m_q.push_front( ppn );
		else
			m_q.push_back(ppn);
		return true;
	}

	bool empty()
	{
		return m_q.empty();
	}

	unsigned int size()
	{
		return (unsigned int)m_q.size();
	}

	void pop()
	{
		if( m_q.empty() )
		{
			return;
		}
		A *Item = m_q.front();
		m_pool.free( Item );
		true_del( (*Item)() );
		m_q.pop_front();
	}

	A* front()
	{
		A *pItem = m_q.front();
		while( false == this->if_in( (*pItem)() ) )
		{
			pop();
			if( m_q.empty() )
				return 0;
			pItem = m_q.front();
		}
		return pItem;
	}
	unsigned int get_mem_size()
	{//k
		return sizeof(A)*m_pool.GetAllCount()/1024;
	}

private:
	MY_POOL    m_pool;
	IN_QUEUE   m_q;
};

template <typename A,typename key >
class XNotReenterQ : public InterfaceXNotReenterQ<A,key,SgsStl::hash_map<key,char> >
{
public:
	XNotReenterQ()
	{
		//BOOST_MPL_ASSERT( (boost::is_class<A>) );
		//BOOST_MPL_ASSERT_NOT( (boost::is_pointer<A>) );
	}
	~XNotReenterQ(){}
};
template <typename A,typename key >
class XNotReenterQ_LitMem : public InterfaceXNotReenterQ<A,key,map<key,char> >
{
public:
	XNotReenterQ_LitMem()
	{
		//BOOST_MPL_ASSERT( (boost::is_class<A>) );
		//BOOST_MPL_ASSERT_NOT( (boost::is_pointer<A>) );
	}
	~XNotReenterQ_LitMem(){}
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename A,typename MYMAP=map<A,char> >
class InterfaceXNotReenterSimpleQ : public CImpNotReenter<A,MYMAP>
{
protected:
	typedef queue<A>                 IN_QUEUE;
public:
	InterfaceXNotReenterSimpleQ()
	{
		////BOOST_MPL_ASSERT(     (boost::is_class<A>) );
		//BOOST_MPL_ASSERT_NOT( (boost::is_pointer<A>) );
	}
	virtual ~InterfaceXNotReenterSimpleQ(){}

public:
	bool push( A obj)
	{
		if( if_in( obj ) )
			return true;

		add( obj );
		m_q.push( obj );
		return true;
	}
    bool empty()
	{
		return m_q.empty();
	}
	unsigned int size()
	{
		return (unsigned int)m_q.size();
	}

	void pop()
	{
		A &item = m_q.front();
		true_del( item );
		m_q.pop();
	}
	A* front()
	{
		A *pItem = &m_q.front();
		while( false == if_in( (*pItem) ) )
		{
			m_q.pop();
			if( m_q.empty() )
				return 0;
			pItem = &m_q.front();
		}
		return pItem;
	}

private:
	IN_QUEUE   m_q;
};

template <typename A>
class XNotReenterSimpleQ : public InterfaceXNotReenterSimpleQ<A,SgsStl::hash_map<A,char> >
{
public:
	XNotReenterSimpleQ()
	{
		//BOOST_MPL_ASSERT( (boost::is_class<A>) );
		//BOOST_MPL_ASSERT_NOT( (boost::is_pointer<A>) );
	}
	~XNotReenterSimpleQ(){}
};
template <typename A>
class XNotReenterSimpleQ_LitMem : public InterfaceXNotReenterSimpleQ<A,map<A,char> >
{
public:
	XNotReenterSimpleQ_LitMem()
	{
		//BOOST_MPL_ASSERT( (boost::is_class<A>) );
		//BOOST_MPL_ASSERT_NOT( (boost::is_pointer<A>) );
	}
	~XNotReenterSimpleQ_LitMem(){}
};
template <typename A, typename XData>
class XNotReenterSimpleQ_WithData : public InterfaceXNotReenterSimpleQ<A,map<A,char> >
{
public:
    XNotReenterSimpleQ_WithData(){}
    ~XNotReenterSimpleQ_WithData(){}

    XData *get_data(A key)
    {
        typename map<A, XData*>::iterator iter = m_pdata.find(key);
        if(m_pdata.end() == iter)
            return 0;
        return iter->second;
    }
    bool set_data(A key, XData *pdata)
    {
        if(0 == pdata)
            return false;
        if(!if_in(key))
            return false;
        m_pdata[key] = pdata;
        return true;
    }
    void pop()
    {
        A* pitem = InterfaceXNotReenterSimpleQ<A,map<A,char> >::front();

        typename map<A, XData*>::iterator iter = m_pdata.find((*pitem));
        if(m_pdata.end() != iter)
            m_pdata.erase(iter);

        InterfaceXNotReenterSimpleQ<A,map<A,char> >::pop();
    }

private:
    map<A,XData*> m_pdata;
};


/***************************************************************************************************************
[name]:  线程安全队列
****************************************************************************************************************/
template<typename A>
class XThreadSaveQueue
{
	typedef std::list<A> IN_QUEUE;
public:
	XThreadSaveQueue()
	{
	}
	~XThreadSaveQueue()
	{
	}

	bool empty()
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		return m_q.empty();
	}

	void push(const A _Val)
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		m_q.push_back( _Val );
	}
	void push_front(const A _Val)
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		m_q.push_front( _Val );
	}

	A pop( bool &ispopok )
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		if( this->empty() )
		{
			ispopok = false;
			return A();
		}
		ispopok = true;
		A item  = m_q.front();
		m_q.pop_front();
		return item;
	}
	A& front()
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		return m_q.front();
	}

	unsigned int size()
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		return m_q.size();
	}
private:
	boost::recursive_mutex m_mutex;
	IN_QUEUE               m_q;
};


/***************************************************************************************************************
	[name]:  可重入队列

    [param]
		key:
		timeouts_msel: 超时设定,millisecond;
		umaxcount:     单次循环的处理数目控制;

	[warning]:
	    <1>提供24天内的准确度，如果超过24天，需要重新启动服务器.

	[subjoin]:
		<1>提供动态的修改影响性能的参数。为以后要添加的负反馈系统提供基本接口。
****************************************************************************************************************/
template <typename key,unsigned int timeouts_msel,unsigned int umaxcount=60>
class XReenterTimeoutsQ
{
protected:
	struct TItem
	{
		TItem(key kk,double curtime):in_time(curtime),k(kk){}
	public:
		double        in_time;
		key           k;
	};

	typedef std::queue<TItem>        IN_TIMEOUTS_QUEUE;
	typedef boost::function<void(key k)>hander_timeouts;
public:
	XReenterTimeoutsQ()
	{
		modify_timeouts(timeouts_msel);
		modify_maxcount(umaxcount);
		in_timer.restart();
	}
	~XReenterTimeoutsQ (){}

public:
	void set_hander_timeouts(hander_timeouts hander)
	{
		m_hander=hander;
	}
	unsigned int get_timeouts( )
	{
	    unsigned int ret = (unsigned int)c_timeouts*1000;
		return ret;
	}
	void modify_timeouts(unsigned int timeoutsmsel)
	{
		c_timeouts = (timeoutsmsel>0?timeoutsmsel:1)*1.0f/1000;
	}
	void modify_maxcount(unsigned int mcount)
	{
		c_maxcount = mcount<1?1:mcount;
	}

	bool add( key k)
	{
#ifdef SGS_AUTO_TEST
		if( m_q.size()>10000 && (0==m_q.size()%200) )
		{
			APPLOG_WARNING("WARNING_ZHT:totalcount="<<m_q.size()<<";classname="<<typeid(key).name()<<FUN_FILE_LINE);
		}
#endif // SGS_AUTO_TEST

		m_q.push( TItem(k,in_timer.elapsed() ) );
		return true;
	}

	void run()
	{
		double ucurtime = in_timer.elapsed();
		if( (m_q.empty()) && (ucurtime>2000) )
			in_timer.restart();

		unsigned int iloop = 0;
		while( (!m_q.empty()) && (iloop<umaxcount) )
		{
			TItem &item = m_q.front();
			if( ucurtime-item.in_time>c_timeouts )
			{
				m_hander( item.k);
				m_q.pop();
				iloop++;
			}else
				break;
		}
	}

	void clear()
	{
		while( !m_q.empty() )
			m_q.pop();
	}
	unsigned int size()
	{
		return m_q.size();
	}
private:
	boost::timer        in_timer;
	unsigned int        c_maxcount;
	double  		    c_timeouts;
	hander_timeouts     m_hander;
	IN_TIMEOUTS_QUEUE   m_q;
};


/************************************************************************************************
	[name]:  不允许重入的队列

	[param]
		key:
		timeouts:      超时设定,second; 单位百分之一秒.
		umaxcount:     单次循环的处理数目控制;

	[warning]:
		<1>不提供线程安全

	[subjoin]: m_hander 要保证这个函数没有异常
************************************************************************************************/
template <typename key,unsigned int timeouts,unsigned int umaxcount,typename IN_MAP_XState=SgsStl::hash_map<key,char> >
class InterfaceXSimpleTimeoutsQ
{
protected:
	struct TItem
	{
		TItem(key kk,unsigned curtime):in_time(curtime),k(kk){}
	public:
		unsigned int  in_time; //单位百分之一秒.
		key           k;
	};

	typedef std::list<TItem>        IN_TIMEOUTS_QUEUE;
	typedef boost::function<void(key k)>hander_timeouts;
public:
	InterfaceXSimpleTimeoutsQ()
	{
		memset(m_debugname,0,sizeof(m_debugname) );
		m_is_circularqueue = false;
		m_maxcount_times   = umaxcount;
		set_timeout_len( timeouts );
	}
	~InterfaceXSimpleTimeoutsQ(){}

public:
	void set_hander_timeouts(hander_timeouts hander)
	{
		m_hander=hander;
	}
	void set_timeout_len( unsigned int param_timeouts )
	{
		c_timeouts = param_timeouts;
	}
	void set_circularqueue()
	{
		m_is_circularqueue = true;
		m_curpos_q         = m_q.begin();
	}
	void set_debugname( string myname )
	{
		CSgsPubFun::strcpy( m_debugname,sizeof(m_debugname),myname.c_str() );
	}
	bool add( key k)
	{
		if( 0!=is_in_queue(k) )
			return false;

		m_map_state.insert( std::make_pair(k,'a') );
		m_q.push_back( TItem(k,g_server_relativetime ) );
		return true;
	}

	void del( key k)
	{
		m_map_state.erase( k );
	}

	bool is_in_queue(key k)
	{
		typename IN_MAP_XState::iterator it = m_map_state.find( k );
		if( it == m_map_state.end() )
			return false;
		return true;
	}
	void clear()
	{
		while( !m_q.empty() )
			m_q.pop();

		m_map_state.clear();
	}
	unsigned int size()
	{
		return m_q.size();
	}
	void run()
	{
		if( m_is_circularqueue )
			run_cc();
		else
			run_nc();
	}

protected:
	void run_nc()
	{
		if( m_q.empty() )
			return;

		try
		{
			unsigned int iloop    = 0;
			while( iloop<m_maxcount_times )
			{
				if( m_q.empty() )
					break;

				TItem &item = m_q.front();
				if( g_server_relativetime-item.in_time>c_timeouts )
				{
					if( 0 != is_in_queue( item.k ) )
						m_hander( item.k);

					del( item.k );
					m_q.pop_front();
					iloop++;
				}else
					break;
			}
		}
		catch (...)
		{
			APPLOG_ERROR("[这里发生了异常] !! m_debugname="<<m_debugname<<typeid(key).name()<<FUN_FILE_LINE );

			//防止一直在一个地方发生异常
			if( !m_q.empty() )
			{
				TItem &item = m_q.front();
				del( item.k );
				m_q.pop_front();
			}
		}
	}
	void run_cc()
	{
		if( m_q.empty() )
			return;

		try
		{
			if( m_q.end() == m_curpos_q )
				m_curpos_q = m_q.begin();

			unsigned int iloop    = 0;
			while( iloop<m_maxcount_times )
			{
				TItem &item = *m_curpos_q;
				if( g_server_relativetime-item.in_time>c_timeouts )
				{
					item.in_time = g_server_relativetime;
					if( 0 != is_in_queue( item.k ) )
					{
						m_hander( item.k );
						++iloop;
						++m_curpos_q;
					}
					else
					{
						del( item.k );
						m_curpos_q = m_q.erase( m_curpos_q );
					}

					if( m_q.end() == m_curpos_q )
						break;
				}else
					break;
			}
		}
		catch (...)
		{
			APPLOG_ERROR("[这里发生了异常] !! m_debugname="<<m_debugname<<typeid(key).name()<<FUN_FILE_LINE );
			m_curpos_q = m_q.begin();
		}
	}


private:
	unsigned int        m_maxcount_times;   //单次循环处理的最多数目
	unsigned int		c_timeouts;         //超时的时间,百分之一秒的精度
	IN_MAP_XState       m_map_state;
	hander_timeouts     m_hander;
	IN_TIMEOUTS_QUEUE   m_q;

private:
	bool                                 m_is_circularqueue;  //是否是循环队列
	typename IN_TIMEOUTS_QUEUE::iterator m_curpos_q;

private:
	char   m_debugname[32]; //为了调试
};

template <typename key,unsigned int timeouts,unsigned int umaxcount=60 >
class XSimpleTimeoutsQ : public InterfaceXSimpleTimeoutsQ<key,timeouts,umaxcount,SgsStl::hash_map<key,char> >
{
public:
	XSimpleTimeoutsQ(){}
	~XSimpleTimeoutsQ(){}
};

template <typename key,unsigned int timeouts,unsigned int umaxcount=60 >
class XSimpleTimeoutsQ_LitMem : public InterfaceXSimpleTimeoutsQ<key,timeouts,umaxcount,map<key,char> >
{
public:
	XSimpleTimeoutsQ_LitMem(){}
	~XSimpleTimeoutsQ_LitMem(){}
};

/************************************************************************************************
	[name]:  不允许重入队列, 队列中的对象有状态信息

	[param]
		key:
		XState:        对象的状态类
		timeouts:      超时设定,second;
		umaxcount:     单次循环的处理数目控制;

	[warning]:
		<1> 不提供线程安全
		<2> XState不能是指针，只能是类,并且需要提供构造，析构函数

	[subjoin]:
************************************************************************************************/

template <typename key,typename XState,unsigned int timeouts,unsigned int umaxcount=100,typename IN_MAP_XState=SgsStl::hash_map<key,XState *> >
class IfXStateTimeoutsQ
{
protected:
	struct TItem
	{
		TItem(key kk,unsigned curtime):in_time(curtime),k(kk){}
	public:
		unsigned int  in_time;
		key           k;
	};

	typedef std::queue<TItem>					IN_TIMEOUTS_QUEUE;
	typedef boost::object_pool<XState>			POOL_XState;
	typedef boost::function<void(key k,XState &state)>hander_timeouts;

public:
	IfXStateTimeoutsQ()
	{
		BOOST_MPL_ASSERT( (boost::is_class<XState>) );
		BOOST_MPL_ASSERT_NOT( (boost::is_pointer<XState>) );

		c_timeouts = timeouts;
	}
	virtual ~IfXStateTimeoutsQ(){}

public:
	void set_hander_timeouts(hander_timeouts hander)
	{
		m_hander=hander;
	}

	XState* add( key k)
	{
#ifdef SGS_AUTO_TEST
		if( m_map_state.size()>10000 && (0==m_map_state.size()%200))
		{
			APPLOG_WARNING("WARNING_ZHT:totalcount="<<m_map_state.size()<<";classname="<<typeid(XState).name()<<FUN_FILE_LINE);
		}
#endif // SGS_AUTO_TEST

		XState *pstate = get_xstate(k);
		if( pstate )
			return pstate;

		pstate = m_XStatePool.construct();
		if( !pstate )
			return 0;
		m_map_state.insert( std::make_pair(k,pstate) );
		m_q.push( TItem(k,g_curserver_time ) );
		return pstate;
	}

	void del( key k)
	{
		XState *pst = get_xstate( k );
		if( pst )
		{
			m_XStatePool.destroy( pst );
		}
		m_map_state.erase( k );
	}

	XState *get_xstate(key k)
	{
		typename IN_MAP_XState::iterator it = m_map_state.find( k );
		if( it == m_map_state.end() )
			return 0;
		return it->second;
	}

	void run()
	{
		unsigned int ucurtime = g_curserver_time;
		XState *pst = 0;
		unsigned int iloop = 0;
		while( (!m_q.empty()) && (iloop<umaxcount) )
		{
			TItem &item = m_q.front();
			if( ucurtime-item.in_time>c_timeouts )
			{
				if( !m_hander.empty() )
				{
					pst = get_xstate( item.k );
					if( pst )
						m_hander( item.k,*pst);
				}

				del( item.k );
				m_q.pop();
				iloop++;
			}else
				break;
		}
	}

private:
	unsigned int		c_timeouts;
	POOL_XState         m_XStatePool;
	IN_MAP_XState       m_map_state;
	hander_timeouts     m_hander;
	IN_TIMEOUTS_QUEUE   m_q;
};


template <typename key,typename XState,unsigned int timeouts,unsigned int umaxcount=100>
class XStateTimeoutsQ : public IfXStateTimeoutsQ<key,XState,timeouts,umaxcount,SgsStl::hash_map<key,XState *> >
{
public:
	XStateTimeoutsQ(){}
	~XStateTimeoutsQ(){}
};
template <typename key,typename XState,unsigned int timeouts,unsigned int umaxcount=100>
class XStateTimeoutsQ_LitMem : public IfXStateTimeoutsQ<key,XState,timeouts,umaxcount,map<key,XState *> >
{
public:
	XStateTimeoutsQ_LitMem(){}
	~XStateTimeoutsQ_LitMem(){}
};

#endif	//__SGS_COMMENT_TIMEOUTSQ__
