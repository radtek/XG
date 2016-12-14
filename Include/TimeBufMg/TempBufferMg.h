#ifndef TempBufferMg_h__
#define TempBufferMg_h__
#include "./TempBufferObjDefin.h"


class CTempMsgBufferMg
{
public:
	static CTempMsgBufferMg * single(){ static CTempMsgBufferMg _check; return &_check; }

private:
	CTempMsgBufferMg(){}
public:
	~CTempMsgBufferMg(void){}

public:
	TItemMsgItem        *add( string        ukey,const char *pdata, unsigned int datalen, unsigned int maxkeeptimelen/*百分之一秒100就是一秒*/  );
	TItemMsgItem        *add( unsigned int &ukey,const char *pdata, unsigned int datalen, unsigned int maxkeeptimelen/*百分之一秒100就是一秒*/  );
    bool                 del( string vkey );
	bool                 del( unsigned int vkey );
	void                 del_uu( unsigned int vkey ){ this->del(vkey); }
	void                 delvoid( unsigned int vkey ){del(vkey);}
	void                 auto_del_delay( unsigned int ukey );

	const TItemMsgItem * find( string vkey );
	const TItemMsgItem * find( unsigned int vkey );

	void                 run();
	void                 uninit();

	void                 setcbfun_msguint( HandMsgBuf_TimeoutKeyUint  msgcb_uint);
	void                 setcbfun_msgstr(  HandMsgBuf_TimeoutKeyStr   msgcb_str);
public:

};


//////////////////////////////////////////////////////////////////////////////
class CTempObjMg
{
public:
	static CTempObjMg * single(){ static CTempObjMg _check; return &_check; }

private:
	CTempObjMg(){}
public:
	~CTempObjMg(void){}

public:
	bool 	             add( string        ukey, const  CTempObjItem *pObj,unsigned int maxkeeptimelen/*百分之一秒100就是一秒*/  );
	bool                 add( unsigned int &ukey, const  CTempObjItem *pObj,unsigned int maxkeeptimelen/*百分之一秒100就是一秒*/  );
	bool                 del( string vkey );
	void                 del_tt( string vkey ){ this->del(vkey); }
	bool                 del( unsigned int vkey );
	bool                 del_uu( unsigned int vkey ){ return this->del(vkey);}

	const CTempObjItem * find( string vkey,bool isLog=true );
	const CTempObjItem * find( unsigned int errorid,bool isLog=true );

	void                 setcbfun_objuint( HandObjBuf_TimeoutKeyUint  objcb_uint);
	void                 setcbfun_objstr(  HandObjBuf_TimeoutKeyStr   objcb_str );
	
private:

};
void auto_del_msgbuf_keystr(  string strv );
void auto_del_msgbuf_keyuint( unsigned int vv );
void auto_del_objbuf_keystr(  string strv );
void auto_del_objbuf_keyuint( unsigned int vv );



template<typename XMsg>
const XMsg *ggfun_TempBuf_GetXMsgByUKey( unsigned int ukey,unsigned int OPCode, bool isAutoDel )
{
	const TItemMsgItem *pBufItem = CTempMsgBufferMg::single()->find( ukey );
	if( !pBufItem )
		return 0;

	if( pBufItem->m_bufflen<sizeof(XMsg) )
	{
		APPLOG_ERROR("[Logic error, bufsize<msglen] msgname="<<typeid(XMsg).name()<<";ukey="<<ukey<<";OPCode="<<OPCode<<FUN_FILE_LINE );
		return 0;
	}

	const XMsg *pCMsg = (const XMsg*)( pBufItem->m_pbuff );
	if( OPCode != pCMsg->OPCode )
	{
		APPLOG_ERROR("[Logic error, OPCode != pCMsg->OPCode] msgname="<<typeid(XMsg).name()<<";ukey="<<ukey<<";OPCode="<<OPCode<<";pCMsg->OPCode="<<pCMsg->OPCode<<FUN_FILE_LINE );
		return 0;
	}
	if( isAutoDel )
		CTempMsgBufferMg::single()->auto_del_delay( ukey );
	return pCMsg;
}

#endif