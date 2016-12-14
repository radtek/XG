#ifndef _SGS_FRAME_BASECONFIG_
#define _SGS_FRAME_BASECONFIG_

#include "../../Include/Common/winflat.h"
#include <boost/lexical_cast.hpp>

typedef map<string,string> MAP_CONFIG;
class CBaseConfig
{
public:
	static CBaseConfig *single();
private:
	CBaseConfig(){}
public:
	bool            loadxmlconfig( string xmlfile,bool ismain );
    void            unload();

public:
	string          getstring( string file,string field,string key, string def="" );
	unsigned int    getuint(	  string file,string field,string key, unsigned int def=0 );
	unsigned short  getsuint(  string file,string field,string key, unsigned short def=0 );
	int             getint(	  string file,string field,string key, int def=0);
	float           getfloat(  string file,string field,string key, float def=0.0f);
	char            getchar(	  string file,string field,string key, char def=0);
    
	void            print();
private:
	bool            loop_load_xmlvalue( class TiXmlElement *pitem, string parentkey );
	string          _getstring(string file,string field,string key);

private:
	MAP_CONFIG      m_mapconfig;
	string          m_strmainfile;
};

#endif