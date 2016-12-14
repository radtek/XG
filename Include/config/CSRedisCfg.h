#ifndef CSRedisCfg_h__
#define CSRedisCfg_h__

#include "../Common/SaveFun.h"

//临时结构信息
class CSRedisCfg
{
private:
	CSRedisCfg(){}
public:
	~CSRedisCfg(void){}

	static      CSRedisCfg &single();
public:
	void        initCfg( string strMainXmlFile );
	int         Seting_XServer_ServerIpport_toredis( string strkey );
	int         Seting_XServer_ServerIpport_toredis_http( string strkey,string xmlFieldName );
	int         get_cs_serverid_ipport(  string strkey, unsigned int &serverid,string &strip,unsigned short &iport );
private:
	int         set_cs_ipport( string strkey,unsigned int serverid,string &strip,string &strport );
	int			SCFG_WriteString(string strkey, string &strvalue );
	string		SCFG_GetString(  string &strkey );
	struct		redisContext*get_scfg_redis();
};

#endif
