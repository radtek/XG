#ifndef ZJsonPub_h__
#define ZJsonPub_h__
#include "./SaveFun.h"
#include "../json/json.h"

class CJsonPub
{
public:
	static int			Json_GetStr( const Json::Value &jsObj,string strField, string &strVV );
	static string		Json_GetStr( const Json::Value &jsObj,string strField );
	static string		Json_GetStrUtf8ToGbk( const Json::Value &jsObj,string strField );

	static int			Json_GetInt( const Json::Value &jsObj,string strField, int &iVV );
	static int			Json_GetInt( const Json::Value &jsObj,string strField );
	static int			Json_GetInt64(  Json::Value &jsObj,string strField, i64 &iVV );
	static i64			Json_GetInt64(  Json::Value &jsObj,string strField );

	static int			Json_GetUInt( const Json::Value &jsObj,string strField, unsigned int &iVV );
	static unsigned int Json_GetUInt( const Json::Value &jsObj,string strField );
	static unsigned int Json_GetUInt( Json::Value &jsObj,string strFA,string strFB );
	static int			Json_GetUInt64( const Json::Value &jsObj,string strField, ui64 &iVV );
	static ui64			Json_GetUInt64( const Json::Value &jsObj,string strField );

	static string		FormatJsonParam( string &strJs );
	static string		FormatJsonParam( const char *pStr );
	static string		JsonToString( Json::Value &jsItem );
	static void			JsonToString( string &strRet,Json::Value &jsItem );
	static int			JsonToString( TBufferItem &tBuf,Json::Value &jsItem,int icalPos );
	static int			StrToJson( string &strJs,Json::Value &jsDes );
	static int			StrToJsonB( const char *pBuffer,Json::Value &jsDes );
};

#endif
