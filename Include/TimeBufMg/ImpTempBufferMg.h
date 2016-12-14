#ifndef TempObjMg_h__
#define TempObjMg_h__
#include "./TempBufferObjDefin.h"

class ImpTempBufObjMg
{
private:
	friend class CTempMsgBufferMg;
	friend class CTempObjMg;
	static ImpTempBufObjMg * single(){ static ImpTempBufObjMg _check; return &_check; }

private:
	ImpTempBufObjMg();
public:
	~ImpTempBufObjMg(void);

	void			 run();
	void             uninit();

public:
	TTempItemHead *  addbuf( string vkey, const char *pdata,unsigned int datalen,unsigned int maxkeeptimelen=100/*百分之一秒100就是一秒*/ );
	TTempItemHead *  addbuf( unsigned int &vkey,const char *pdata,unsigned int datalen,unsigned int maxkeeptimelen=100/*百分之一秒100就是一秒*/ );

	TTempItemHead *  addobj( string vkey,const CTempObjItem *pObj,unsigned int maxkeeptimelen=100/*百分之一秒100就是一秒*/ );
	TTempItemHead *  addobj( unsigned int &vkey,const CTempObjItem *pObj, unsigned int maxkeeptimelen=100/*百分之一秒100就是一秒*/ );

	bool			 del( string vkey );
	bool			 del( unsigned int vkey );
	TTempItemHead *  find( string vkey );
	TTempItemHead *  find( unsigned int vkey );

	void             setcbfun_msguint( HandMsgBuf_TimeoutKeyUint  msgcb_uint){ m_msgcb_uint = msgcb_uint;  }
	void             setcbfun_msgstr(  HandMsgBuf_TimeoutKeyStr   msgcb_str){  m_msgcb_str  = msgcb_str;   }

	void             setcbfun_objuint( HandObjBuf_TimeoutKeyUint  objcb_uint){ m_objcb_uint = objcb_uint;  }
	void             setcbfun_objstr(  HandObjBuf_TimeoutKeyStr   objcb_str){  m_objcb_str  = objcb_str;   }

private:
	void			 run_id(int maxcount );
	void			 run_str(int maxcount );

	void             cb_timeout_str(  string     strkey, TTempItemHead *pItem );
	void             cb_timeout_uint( unsigned int ukey, TTempItemHead *pItem );

private:
	unsigned int    getnewkey();
	TTempItemHead * createitem( const char *pdata,unsigned int datalen,unsigned int maxkeeptimelen );
	void            destoryitem( TTempItemHead *pItem );

private:
	MAPIDD_VOID		m_mapid;
	MAPKEY_VOID		m_mapstr;

	unsigned int    m_lastchecktime;
	unsigned int    m_curkeyindex;

	HandMsgBuf_TimeoutKeyUint  m_msgcb_uint;
	HandMsgBuf_TimeoutKeyStr   m_msgcb_str;

	HandObjBuf_TimeoutKeyUint  m_objcb_uint;
	HandObjBuf_TimeoutKeyStr   m_objcb_str;
};


#endif