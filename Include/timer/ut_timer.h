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
    /// Remark: ʱ���¼�������ʱ����, ͨ��ʱ�Ӷ���ID�������ֲ�ͬ��ʱ��
    /// @param unsigned int  dwIDEvent : ʱ�Ӷ���ID
    /// @return void :
    //////////////////////////////////////////////////////////////////////////
    virtual void OnTimer(unsigned int  dwIDEvent)=0;
};


/************************************************************************/
/* �û����õ�Timer����                                                  */
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

    ui64 m_qwNextTime;       ///< ʱ����һ�δ�����ʱ��(ϵͳ�����������ms��)
    unsigned int     m_dwElapse;         ///< �������(��λms)
    unsigned int     m_dwId;             ///< ��ʱ�����
    bool             m_bErased;          ///< ȡ�����
    ITimerSink      *m_pTimerSink;      ///< ��ʱ���ص�����ָ��
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
/* ˵��:                                                                */
/*      . ��ģ������û���߳���ѯ, ʱ�ӵĴ������������ڱ����õ�Ƶ��    */
/*      . ��ʱ���Ĵ���������΢��������ʱ��, ������wall_time             */
/*      . ʱ�Ӵ�������������Ϊ���߳�����������                          */
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

    // ��ʼ��
    bool Init(unsigned int  dwTimerCount);

    // ���ü�ʱ��
    bool  SetTimer(ITimerSink* pTimer,unsigned int  uIDEvent,unsigned int  uElapse);
    // ȡ����ʱ��
    void  KillTimer(unsigned int  uIDEvent);
    // ���м�ʱ��
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
