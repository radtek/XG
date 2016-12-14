#ifndef HttpSs_h_REPMSG
#define HttpSs_h_REPMSG
#include "../Common/SaveFun.h"
#include "../../Include/xframe_protocol/xframe_server_pro.h"

class CHttpRep
{
public:
	CHttpRep(void);
	virtual ~CHttpRep(void);

public:
	int				gfun_http_sendrepmsg( unsigned conid,const char *pData,int uDataLen );

private:
	string			InFun_BuildHttpHead();
};

#endif // HttpSs_h_REPMSG
