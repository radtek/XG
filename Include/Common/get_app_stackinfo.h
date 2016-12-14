/********************************************************************
	created:	13:3:2011   17:54
	author:		zhanghongtao
	
	purpose:	¥¶¿Ìc++“Ï≥£
*********************************************************************/
#ifndef get_app_stackinfo_h__
#define get_app_stackinfo_h__

class CPlusExp
{
private:
	CPlusExp( struct _EXCEPTION_POINTERS *pEP );
	~CPlusExp( );
public:
	static void        MapExpToPlusExp();

#ifdef WIN32
	static void _cdecl DowithExp( unsigned int uEC, struct _EXCEPTION_POINTERS *pEP );
#endif // WIN32
};

extern bool is_load_debugfile();
#endif // get_app_stackinfo_h__