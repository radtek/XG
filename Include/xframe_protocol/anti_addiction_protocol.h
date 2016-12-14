/****************************************************************************************************
created:	2010-9-15   16:55
filename: 	F:\game_server_v3\CS\anti_addicted\anti_addiction_protocol.h
author:		Zhanghongtao

purpose:	防沉迷协议
*****************************************************************************************************/
#ifndef sgs_server_anti_addiction_protocol_v1_h__
#define sgs_server_anti_addiction_protocol_v1_h__
#include "../Include/Common/SaveFun.h"
#include <boost/function.hpp>

typedef boost::function<void(unsigned int user_temp_id,int iOnlineMinutes,int iOfflineMinutes)>hand_cb_fcminfo_togs;

struct TUserFCMItem
{
	TUserFCMItem(unsigned int tempid )
	{
		iOnlineMinutes	= 0;
		iOfflineMinutes = 0;
		usertempid      = tempid;
		memset(account,0,sizeof(account) );
	}
	~TUserFCMItem(){ iOnlineMinutes=iOfflineMinutes=0;memset(account,0,sizeof(account) ); }
public:
	char         account[48];
	char         real_pt[40];
	unsigned int usertempid;
	int			 iOnlineMinutes;
	int			 iOfflineMinutes;
};

/* iState
玩家状态：
1000    不在线
1001	没有申请过防沉迷
1002 	身份证信息无效的玩家
1003	成年玩家
1004	身份证信息有效的未成年玩家
*/

/*
struct TCBFCMItem
{
	TCBFCMItem( const char*_szUserID,int _iOnlineMinutes,int _iOfflineMinutes)
	{
		CSgsPubFun::strcpy(szUserID,sizeof(szUserID),_szUserID );
		iOnlineMinutes	= _iOnlineMinutes;
		iOfflineMinutes = _iOfflineMinutes;
	}
	~TCBFCMItem(){}
public:
	char	szUserID[48];
	int		iOnlineMinutes;
	int		iOfflineMinutes;
};*/


namespace anti_addiction
{
	enum em_user_anti_state
	{
		emuser_antistate_initing=1,	//正在初始化中 
		emuser_antistate_fcm,		//需要防沉迷
		emuser_antistate_nofcm,		//不需要防沉迷
	};
}

#endif // sgs_server_anti_addiction_protocol_v1_h__