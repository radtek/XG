#ifndef deathcheck_h__
#define deathcheck_h__
#include "./xframe_types.h"
#include "./SaveFun.h"

struct TCheckFun
{
	unsigned int	 totalcallcount;   //���õĴ���
	ui64 totaltimelen;     //���õ�ʱ��
	ui64 tmlen_last;       //���һ�ε��õ�ʱ��

	unsigned int	 totalcallcount_last;   //���һ���ӵ��õĴ���
	ui64 totaltimelen_last;     //���һ���ӵ��õ�ʱ��

	TCheckFun()
	{
		tmlen_last          = 0;
		totalcallcount	    = 0;
		totaltimelen   	    = 0;
		totalcallcount_last = 0;
		totaltimelen_last   = 0;
	}
};
typedef map<string, TCheckFun*>MAP_CHECKFUN;

class CDeathCheck
{
public:
	static CDeathCheck & single();
private:
	CDeathCheck(void);
	~CDeathCheck(void);
public:
	void		update(string funname,ui64 timelen);
	TCheckFun * find( string funname );
	void        print();

private:
	MAP_CHECKFUN  m_mapfun;
};

class CHelpDeathCheck
{
public:
	CHelpDeathCheck( string funname );
	CHelpDeathCheck( unsigned int opcode );
	CHelpDeathCheck( string funname,bool isautoprint,unsigned int param );
	~CHelpDeathCheck(void);

public:
	unsigned int get_usingtime();
private:
	void        printself();

private:
	bool              m_isautoprint;
	string            m_funname;
	ui64              m_begin;
	unsigned int      m_param;
};

#define  HELPDEATHCHECK(MSG)\
	CHelpDeathCheck helpDeathCheck( MSG )

#define  HELPDEATHCHECK_AUTOPT(MSG,ISPT,COUNT)\
	CHelpDeathCheck helpDeathCheck( MSG,ISPT,COUNT )

#define  HELPDEATHCHECK_NETMSG(OPCODE)\
	CHelpDeathCheck helpDeathCheck( OPCODE )

#endif // deathcheck_h__