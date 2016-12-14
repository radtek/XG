#ifndef RedisFace_h__
#define RedisFace_h__
#include "../Common/SaveFun.h"
#include "./ImpRedis.h"

class CFaceRedis
{
public:
	CFaceRedis(void);
	~CFaceRedis(void);

public:
	int		Init( string strConInfo );

public:
	int     Cmd_GetBinData( string strkey,char *pBuffer,int bufferlen, int &datalen );
	int     Cmd_SetBinData( string strkey, const char *pBuffer, unsigned int bufLen,int expiresecond=0 );
	int     Cmd_Ttl( string strkey,unsigned int &expireSecond );
	int     Cmd_Del( string strkey );

	//如果发现断开了重新连接
	void	CheckAndConRedis();

private:
	class ImpRedis *m_pRedis;
	string			ConnectInfo;//
};

#endif // tt_redis_h__
