/********************************************************************
	created:	28:2:2011   10:32
	author:		zhanghongtao
	
	purpose:	�ṩ��gt���������������
*********************************************************************/

#ifndef NoGtHeartCheck_h__
#define NoGtHeartCheck_h__

#include "../../Include/Common/winflat.h"
#include "../../Include/Common/pool_smallblock.h"
#include <boost/function.hpp>

typedef boost::function< void( unsigned int usertempid) > hander_ngtuser_timeout;

struct TNoGtHeartInfo
{
	enum emHeartMark
	{
		IS_DELETE         = 1<<0,  //�Ƿ��Ѿ���ɾ��
	};

	TNoGtHeartInfo(unsigned int usertempid)
	{
		extern unsigned int g_curserver_time;
		last_check_hearttime			 = g_curserver_time;
		m_last_hearttime				 = g_curserver_time;
		m_last_send_heartmsg_time_toxx	 = g_curserver_time;
		user_tempid						 = usertempid;
		m_is_mark						 = 0;
	}
	~TNoGtHeartInfo(){}
public:
	bool is_marktate(    emHeartMark state){ return (m_is_mark&state)>0; }
	void set_markstate(  emHeartMark state ){ m_is_mark |= state;        }
	void clear_markstate(emHeartMark state ){ m_is_mark &= (~state);     }
public:
	unsigned int  last_check_hearttime;				//���һ�μ��������ʱ��
	unsigned int  m_last_hearttime;					//���һ���յ���Ϣ����ʱ��
    unsigned int  m_last_send_heartmsg_time_toxx; //csʹ��
	unsigned int  user_tempid;
private:
	unsigned char m_is_mark;             //����λ 
};

class CNoGtHeartCheck
{

public:
	static CNoGtHeartCheck * single(){ static CNoGtHeartCheck _check; return &_check; }

private:
	CNoGtHeartCheck();
public:
	~CNoGtHeartCheck(void);

	void			set_handertimeout(hander_ngtuser_timeout hand,unsigned int modifytimeout );
	void			run(unsigned int nowtime );

public:
	bool			add_new_user( unsigned int usertempid );
	void            del_user(     unsigned int usertempid );
    void            update_hearttime( unsigned int usertempid );

	//���ķ���������
	void            me_send_heartmsg_toxxxx(TNoGtHeartInfo *pheartinfo);

private:
	void            del_user(     TNoGtHeartInfo *pInfo   );
	TNoGtHeartInfo *get_heartinfo(   unsigned int usertempid );


private:
	typedef list<TNoGtHeartInfo*>               ListHeartCheck;
	//usertempid 
	typedef map<unsigned int,TNoGtHeartInfo*>   Map_THeartInfo;

	ListHeartCheck			m_listheart;
	Map_THeartInfo			m_mapheart;

	hander_ngtuser_timeout  m_handertimeout;

	unsigned int            m_check_maxtime;        //�೤ʱ�������һ��  
	unsigned int            m_hearttimeout_maxlen;  //���������ʱʱ��
};

#endif // NoGtHeartCheck_h__