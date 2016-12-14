/********************************************************************
	created:	29:10:2015   19:42
	filename: 	F:\ZtyServer\XFrame\Frame\mongdb_hiredis_mg\HiredisThMg.h
	author:		ZhangHongtao  email: 1265008170@qq.com
	
	purpose:	
*********************************************************************/
#ifndef HiredisWork_h__
#define HiredisWork_h__
#include "../Common/SaveFun.h"
#include "../../Include/Common/timeouts_queue.hpp"
#include "../../Include/ServerLib/Thread.h"
#include "../allocmem/alloc_basemg.h"
#include "../hiredis/RedisFace.h"

using namespace snda;

class HiredisWork : public CThread
{
public:
	HiredisWork(class HiredisThMg *pRedisCliMg);
	~HiredisWork();

public:
	int		connectRedis(string ConnInfo);

protected:
	bool	CheckRedisConnect( bool isexe_reconnect=false);
    void	ExecuteOne(bool &iswaiting);

private:
	class HiredisThMg	*	m_pRedisCliMg;
	CFaceRedis *			m_pMyRedisConn;
	unsigned                m_uLastCheckTime;
};
#endif
