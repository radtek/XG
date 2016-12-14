#pragma once
#include "ut_timer.h"

#include <boost/function.hpp>
#include <boost/bind.hpp>

typedef void (*OnMyTimer)();
class CTimerMg : public ITimerSink
{
	typedef boost::function<void()>hander_ontimer;
	typedef map<unsigned int ,hander_ontimer> Map_Hander;
public:
	static CTimerMg *Single();
private:
	CTimerMg(void);
	~CTimerMg(void);

public:
	virtual void OnTimer(unsigned int dwIDEvent);
	
	bool Init();
	void UnInit();
	bool Run();
	bool AddTimer(unsigned int uTimeID,unsigned int uElapse,OnMyTimer hander );
	bool AddTimer(unsigned int uTimeID,unsigned int uElapse,hander_ontimer hander );
	bool DelTimer(unsigned int uTimeID);
private:
	Map_Hander   m_map;
};

extern void DebugMainThreadLive();
