#ifndef UT_TIMER_H
#define UT_TIMER_H

#include "../../Include/Common/xframe_types.h"
#include "../../Include/Common/winflat.h"
#include "../../Include/Common/SimpleObjJar.h"

/************************************************************************/
/*                                                                      */
/************************************************************************/
class ITimerSink
{
public:
    //////////////////////////////////////////////////////////////////////////
    /// Method: virtual public  ITimerSink::OnTimer /*=0*/
    /// Remark: 时钟事件被触发时调用, 通过时钟对象ID可以区分不同的时钟
    /// @param unsigned int  dwIDEvent : 时钟对象ID
    /// @return void :
    //////////////////////////////////////////////////////////////////////////
    virtual void OnTimer(unsigned int  dwIDEvent)=0;
};


/************************************************************************/
/* 用户设置的Timer对象                                                  */
/************************************************************************/
struct STimer
{
    //////////////////////////////////////////////////////////////////////////
    // CREATORS
    //////////////////////////////////////////////////////////////////////////

    STimer(): m_qwNextTime(0), m_dwElapse(0), m_dwId(0), m_bErased(false), m_pTimerSink(0) {}
    ~STimer() { m_pTimerSink = 0; }

    //////////////////////////////////////////////////////////////////////////
    // MANIPULATORS
    //////////////////////////////////////////////////////////////////////////

    bool operator>(const STimer& second) { return (m_qwNextTime > second.m_qwNextTime); }

    //////////////////////////////////////////////////////////////////////////
    // VARIABLES
    //////////////////////////////////////////////////////////////////////////

    ui64 m_qwNextTime;       ///< 时钟下一次触发的时间(系统启动后的运行ms数)
    unsigned int     m_dwElapse;         ///< 触发间隔(单位ms)
    unsigned int     m_dwId;             ///< 计时器编号
    bool             m_bErased;          ///< 取消标记
    ITimerSink      *m_pTimerSink;      ///< 计时器回调对像指针
};


/************************************************************************/
/* struct TimeGret                                                      */
/************************************************************************/
struct FTimeGreat
{
    bool operator()(const STimer* _Left, const STimer* _Right) const { return (_Left->m_qwNextTime > _Right->m_qwNextTime); }
};


/************************************************************************/
/* class CTimerQue                                                      */
/************************************************************************/
class CTimerQue
{
public:
    //////////////////////////////////////////////////////////////////////////
    // CREATORS
    //////////////////////////////////////////////////////////////////////////

    CTimerQue(void);
    ~CTimerQue(void);

    //////////////////////////////////////////////////////////////////////////
    // MANIPULATORS
    //////////////////////////////////////////////////////////////////////////

    void Push(STimer* pNewTimer);
    void Pop();
    STimer* Top();
    void Erase(unsigned int  iId);

private:
    //////////////////////////////////////////////////////////////////////////
    // VARIABLES
    //////////////////////////////////////////////////////////////////////////

    std::vector<STimer*> m_vecTimer;
};


/************************************************************************/
/* class CSDTimerModule                                                 */
/* 说明:                                                                */
/*      . 本模块自身并没有线程轮询, 时钟的触发精度依赖于被调用的频率    */
/*      . 定时器的触发依赖于微处理器的时间, 而不是wall_time             */
/*      . 时钟触发次数不会因为主线程阻塞而减少                          */
/************************************************************************/
class CSDTimerModule
{
public:
	static CSDTimerModule * InstanceEx()
	{
		static CSDTimerModule md;
		return &md;
	}
public:
    //////////////////////////////////////////////////////////////////////////
    // MANIPULATORS
    //////////////////////////////////////////////////////////////////////////

    // 初始化
    bool Init(unsigned int  dwTimerCount);

    // 设置计时器
    bool  SetTimer(ITimerSink* pTimer,unsigned int  uIDEvent,unsigned int  uElapse);
    // 取消计时器
    void  KillTimer(unsigned int  uIDEvent);
    // 运行计时器
    bool  Run();

protected:
    //////////////////////////////////////////////////////////////////////////
    // CREATORS
    //////////////////////////////////////////////////////////////////////////

    CSDTimerModule(void);
    virtual ~CSDTimerModule(void);

private:
    //////////////////////////////////////////////////////////////////////////
    // VARIABLES
    //////////////////////////////////////////////////////////////////////////
    CTimerQue                   m_oTimerQue;
	sgs_comment::CSimpleObjJar<STimer>	m_oTimerPool;
};

#endif  // UT_TIMER_H
