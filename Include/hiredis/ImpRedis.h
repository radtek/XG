#ifndef ImpRedis_h__
#define ImpRedis_h__

#include "../Common/SaveFun.h"
#include "../log/interfacelog.h"
#include "../hiredis/hiredis.h"

class ImpRedis
{
public:
	ImpRedis(void);
	~ImpRedis(void);

public:
	int     Init(string strConInfo);
private:
	void    run();

public:
	int     Cmd_SetBinData( string &strkey, const char *pBuffer, size_t bufLen, unsigned int expiresecond );
	int     Cmd_GetBinData( string &strkey,char *pBuffer,int bufferlen, int &datalen );
	int     Cmd_Ttl( string &strkey,unsigned int &expireSecond );
	int     Cmd_Del( string &strkey );
	int		Cmd_ping();

	void	CheckAndConRedis();

private:
	int     ExecConnect();
	void    HappenError( int iErrPos ); //发生了错误，重置
	void	Cmd_quit();

private:
	redisContext  *	m_pRedis;
	char			m_RedisIp[64];
	int				m_RedisPort;
	unsigned        m_uLastConTm;  //最后一次连接的时间
};

#endif // tt_redis_h__
