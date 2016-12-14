/********************************************************************
	created:	28:12:2010   10:59
	author:		zhanghongtao
	
	purpose:	�������������رտ�����Ϣ
*********************************************************************/

#ifndef SgsServerControlInfo_h__
#define SgsServerControlInfo_h__
#include "../Common/SaveFun.h"

enum emServerState
{
	server_state_starting=1,	//������
	server_state_working,       //��������
	server_state_shuting,		//�ر���
	server_state_shut,          //���Թر�
};
class CControlerServer
{
public:
	CControlerServer();
	~CControlerServer();

public:
	bool    		is_starting(){ return server_state_starting==m_serverstate; }
	bool    		is_working(){  return server_state_working==m_serverstate;  }
	bool    		is_shuting(){  return server_state_shuting==m_serverstate;  }
	bool    		is_shut();
	
	unsigned int	get_statetime_len();
	void			set_state( emServerState v);
	
	bool			is_localregion(){ return m_localregion; }
	void			set_localregion( bool isset ){ m_localregion=isset; }
private:
	emServerState   m_serverstate;
	unsigned int    m_state_begintime;
	bool            m_localregion;
};

extern CControlerServer  g_controlserver;
#endif // SgsServerControlInfo_h__