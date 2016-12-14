/** @file SQLDB.h @brief MSSQL和MySQL接口封装。
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

#pragma once

#include "../../Include/Common/winflat.h"
#ifdef WIN32
#include "winsock2.h"        //由于mysql.h中未定义__LCC__，因此需要这里包含winsock.h
#endif
//#include "../mysql/mysql.h"
#include "../../../XDepend/MySQL_5.5/include/mysql.h"

namespace snda
{

/**
@brief 封装MySQL的记录集
*
*	封装MySQL的记录集，不可由多个线程同时操作本对象实例
*/

    class CMySQLRecordSet
    {
        friend class CMySQLConnection;
    public:
        ///构造函数
        CMySQLRecordSet(): m_res(NULL), m_row(NULL)
        {
			m_mysql     = 0;
			m_istimeout = false;
        }
        
        ///析构函数
        ~CMySQLRecordSet()
        {
			if( m_mysql )
			{
				if( m_res )
					mysql_free_result(m_res);
				int status =0;
				do{
					status = mysql_next_result(m_mysql);
					if( 0!=status )
						break;
					m_res = mysql_store_result(m_mysql);
					if( m_res ) mysql_free_result(m_res);
				}while(0 == status);
				m_res	= 0;
				m_mysql = 0;
			}
        }
        
        ///获取记录个数
        unsigned int GetRecordCount() { return (m_res ? (unsigned int)mysql_num_rows(m_res) : 0); }
        
        ///获取字段个数
        unsigned int GetFieldCount() { return (m_res ? (unsigned int)mysql_num_fields(m_res) : 0); }
        
        
        ///获取下一个记录行
        bool GetRecord()
        {
            if (m_res == NULL)
                return false;
            m_row = mysql_fetch_row(m_res);
            return (m_row != NULL);
        }

		///获取下一个记录行
		bool GetRecord( std::vector<unsigned long> &VFieldDataLen)
		{
			if (m_res == NULL)
				return false;
			m_row = mysql_fetch_row(m_res);

			if( m_row )
			{
				unsigned int fieldcount = GetFieldCount( );
				unsigned long * pArray  = mysql_fetch_lengths( m_res );
				for( unsigned int t=0;t<fieldcount; ++t )
				{
					VFieldDataLen.push_back( *pArray );
					pArray++;
				}
			}

			return (m_row != NULL);
		}

        ///获取当前记录行中某一个字段的值
        const char* GetFieldValue(unsigned int nIndex)
        {
            if (m_row == NULL)
                return NULL;
            if (nIndex >= GetFieldCount())
                return NULL;
            return m_row[nIndex];
        }

		void  settimeout(){ m_istimeout=true; }
		bool  istimeout(){ return m_istimeout;}

    private:
		MYSQL    *  m_mysql;
        MYSQL_RES*  m_res;
        MYSQL_ROW   m_row;
		bool        m_istimeout;

    };

/**
@brief 封装MySQL的连接
*
*	封装MySQL的连接，不可由多个线程同时操作本对象实例
*/

    class CMySQLConnection
    {
    public:
        ///定义ExecuteEx的返回值
        typedef enum {Failed, NoRecordSet, HasRecordSet} EExecResult;

        ///构造函数
        CMySQLConnection(void);

        ///析构函数
        ~CMySQLConnection(void);

        ///设置连接信息
        void SetConnectionString(const char* pszServerAddr, int nServerPort,
            const char* pszDBName, const char* pszUserID, const char* pszPassword,const char *pConFormat);

        ///打开连接
        bool Connect(const char* pszServerAddr, int nServerPort,
            const char* pszDBName, const char* pszUserID, const char* pszPassword,
			const char *pConFormat,string &strerror);

        ///打开连接，在设置连接信息后调用
        bool Connect( string &strerror );

        ///关闭连接
        void Close();

        ///是否已连接
        bool IsConnected();

        ///转义字符串，使之可用于MYSQL查询
        char* EscapeString(const char* pszSrc, int nSize, char* pszDest)
        {
            if (m_pSqlCon)
            {
                mysql_real_escape_string(m_pSqlCon, pszDest, pszSrc, nSize);
                return pszDest;
            }
            else
                return NULL;
        }

        ///返回受到最后一个UPDATE、DELETE或INSERT查询影响(变化)的行数
        unsigned int GetAffectedRows() { return (unsigned int)mysql_affected_rows(m_pSqlCon); }

        ///转义字符串，使之可用于MYSQL查询
        char* EscapeString(const char* pszSrc, char* pszDest)
        {
            return EscapeString(pszSrc, (int)strlen(pszSrc), pszDest);
        }
		string EscapeString(const char* pszSrc)
		{
			char buffdes[1024]={0};
			if( strlen(pszSrc)*2+1>sizeof(buffdes) )
				return "";
			EscapeString(pszSrc, (int)strlen(pszSrc), buffdes);
			return string( buffdes );
		}

        ///转义字符串，使之可用于MYSQL查询，主要用在转意2进制数据
        size_t EscapeStringEx(const char* pszSrc, int nSize, char* pszDest)
        {
            if (m_pSqlCon)
            {
                return mysql_real_escape_string(m_pSqlCon, pszDest, pszSrc, nSize);
            }
            else
            {
                return 0;
            }
        }

        ///无差别执行SQL语句，通过返回值判断具体效果
        int  ExecuteEx(const char* pszSQL, CMySQLRecordSet& rcdSet);
        ///无差别执行SQL语句，通过返回值判断具体效果(支持二进制代码数据)
        int  ExecuteEx(const char* pszSQL, int nLength,CMySQLRecordSet& rcdSet);

        ///执行一条SQL语句，不返回记录集
        bool Execute(const char* pszSQL);

        ///执行一条SQL语句，返回记录集
        bool Execute(const char* pszSQL, CMySQLRecordSet& rcdSet);

        ///获取上一次错误号
        unsigned int GetLastError()
        {
            if(m_pSqlCon!=NULL)
            {
                return (unsigned int)mysql_errno(m_pSqlCon);
            }
            return  0;
        }

        ///获取上一次错误描述
        const char* GetLastErrorInfo()
        {
            if(m_pSqlCon!=NULL)
            {
                return mysql_error(m_pSqlCon);
            }
            return NULL;
        }

    private:

        char    m_szServerAddr[24];
        char    m_szDBName[64];
        char    m_szUserID[64];
        char    m_szPassword[32];
		char    m_szConFormat[16];

        int     m_nServerPort;
        MYSQL*  m_pSqlCon;
        bool    m_bConnected;
    };
};
