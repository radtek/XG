/** @file SQLDB.cpp @brief MSSQL和MySQL接口封装的实现
<pre>
*	Copyright (c) 2005，上海盛大网络研发中心
*	All rights reserved.
*
*	当前版本：
*	作    者：休闲平台小组
*	完成日期：2005-03-22
*
*	取代版本：none
*	作    者：none
*	完成日期：none
</pre>*/

#include "../../include/ServerLib/MySQLDB.h"

#ifdef WIN32
#pragma comment(lib, "libmysql.lib")
#endif


snda::CMySQLConnection::CMySQLConnection(void)
{
    memset(m_szServerAddr, 0, sizeof(m_szServerAddr));
    memset(m_szDBName,     0, sizeof(m_szDBName));
    memset(m_szUserID,     0, sizeof(m_szUserID));
    memset(m_szPassword,   0, sizeof(m_szPassword));
	memset(m_szConFormat,  0, sizeof(m_szConFormat));

    m_pSqlCon = mysql_init(NULL);
    m_bConnected   = false;
}

snda::CMySQLConnection::~CMySQLConnection(void)
{
    if (m_pSqlCon)
    {
        mysql_close(m_pSqlCon);
    }
}

void  snda::CMySQLConnection::SetConnectionString(const char* pszServerAddr, int nServerPort,
                                                  const char* pszDBName, const char* pszUserID, const char* pszPassword,const char *pConFormat )
{
    strncpy(m_szServerAddr, pszServerAddr, sizeof(m_szServerAddr) - 1);
    strncpy(m_szDBName, pszDBName, sizeof(m_szDBName) - 1);
    strncpy(m_szUserID, pszUserID, sizeof(m_szUserID) - 1);
    strncpy(m_szPassword, pszPassword, sizeof(m_szPassword) - 1);
	if( pConFormat && strlen(pConFormat) ){
		strncpy( m_szConFormat, pConFormat, sizeof(m_szConFormat)-1 );
	}
    m_nServerPort = nServerPort ;
}

bool snda::CMySQLConnection::Connect(const char* pszServerAddr, int nServerPort,
                                     const char* pszDBName, const char* pszUserID,
									 const char* pszPassword,const char *pConFormat,
									 string &strerror)
{
    SetConnectionString(pszServerAddr, nServerPort, pszDBName, pszUserID, pszPassword,pConFormat);
    return Connect(strerror);
}

bool snda::CMySQLConnection::Connect( string &strerror )
{
    if (m_pSqlCon == NULL)
    {
        m_pSqlCon = mysql_init(NULL);
    }
    if (m_pSqlCon == NULL)
    {
        return false;
    }

    MYSQL* myConn = mysql_real_connect(m_pSqlCon, m_szServerAddr,
        m_szUserID, m_szPassword, m_szDBName, m_nServerPort, NULL, CLIENT_MULTI_STATEMENTS);
    if (!myConn)
    {
		const char *perror = mysql_error(m_pSqlCon);
		strerror = perror;
        Close();
        return false;
    }
	//CLIENT_MULTI_STATEMENTS
	if( 0!=mysql_set_server_option( myConn,MYSQL_OPTION_MULTI_STATEMENTS_ON) )
	{
		//assert(0);
	}
	if( strlen(m_szConFormat)>0 )
		mysql_set_character_set( myConn, m_szConFormat );
    m_bConnected = true;
    return true;
}

void snda::CMySQLConnection::Close()
{
    mysql_close(m_pSqlCon);
    m_pSqlCon = 0;
    m_bConnected   = false;
}

bool snda::CMySQLConnection::IsConnected()
{
    if(!m_bConnected)
        return  false;
    return (mysql_ping(m_pSqlCon)==0);
}

bool snda::CMySQLConnection::Execute(const char* pszSQL)
{
	if( !m_pSqlCon )
		return false;

    return (0 == mysql_query(m_pSqlCon, pszSQL));
}

bool snda::CMySQLConnection::Execute(const char* pszSQL, CMySQLRecordSet& rcdSet)
{
	if( !m_pSqlCon )
		return false;

    if (0 != mysql_query(m_pSqlCon, pszSQL))
        return false;
    if (rcdSet.m_res)
        mysql_free_result(rcdSet.m_res);
	rcdSet.m_mysql  = m_pSqlCon;
    rcdSet.m_res = mysql_store_result(m_pSqlCon);
    return (rcdSet.m_res != NULL);
}

int snda::CMySQLConnection::ExecuteEx(const char* pszSQL, CMySQLRecordSet& rcdSet)
{
	if( !m_pSqlCon )
		return Failed;

    if (0 != mysql_query(m_pSqlCon, pszSQL))
	{
#ifdef _DEBUG
		const char *perror = mysql_error( m_pSqlCon );
		//std<<cout<<perror;
#endif // _DEBUG
		return Failed;
	}
    if( rcdSet.m_res )
        mysql_free_result(rcdSet.m_res);
	rcdSet.m_mysql  = m_pSqlCon;
    rcdSet.m_res    = mysql_store_result(m_pSqlCon);
    if(rcdSet.m_res != NULL)
    {
        return HasRecordSet;       ///Rescord Set
    }

    if(!mysql_field_count(m_pSqlCon))
    {
        return  NoRecordSet;       ///NO Rescord Set
    }
    return  Failed;

}

int  snda::CMySQLConnection::ExecuteEx(const char* pszSQL, int nLength,CMySQLRecordSet& rcdSet)
{
	if( !m_pSqlCon )
		return Failed;

    if (0 != mysql_real_query(m_pSqlCon, pszSQL,nLength))
        return Failed;
    if (rcdSet.m_res)
        mysql_free_result(rcdSet.m_res);
	rcdSet.m_mysql  = m_pSqlCon;
    rcdSet.m_res = mysql_store_result(m_pSqlCon);
    if(rcdSet.m_res != NULL)
    {
        return HasRecordSet;       ///Rescord Set
    }

    if(!mysql_field_count(m_pSqlCon))
    {
        return  NoRecordSet;       ///NO Rescord Set
    }
    return  Failed;
}

