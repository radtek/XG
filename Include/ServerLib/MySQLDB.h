/** @file SQLDB.h @brief MSSQL��MySQL�ӿڷ�װ��
<pre>
*	Copyright (c) 2005���Ϻ�ʢ�������з�����
*	All rights reserved.
*
*	��ǰ�汾��
*	��    �ߣ�����ƽ̨С��
*	������ڣ�2005-03-22
*
*	ȡ���汾��none
*	��    �ߣ�none
*	������ڣ�none
</pre>*/

#pragma once

#include "../../Include/Common/winflat.h"
#ifdef WIN32
#include "winsock2.h"        //����mysql.h��δ����__LCC__�������Ҫ�������winsock.h
#endif
//#include "../mysql/mysql.h"
#include "../../../XDepend/MySQL_5.5/include/mysql.h"

namespace snda
{

/**
@brief ��װMySQL�ļ�¼��
*
*	��װMySQL�ļ�¼���������ɶ���߳�ͬʱ����������ʵ��
*/

    class CMySQLRecordSet
    {
        friend class CMySQLConnection;
    public:
        ///���캯��
        CMySQLRecordSet(): m_res(NULL), m_row(NULL)
        {
			m_mysql     = 0;
			m_istimeout = false;
        }
        
        ///��������
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
        
        ///��ȡ��¼����
        unsigned int GetRecordCount() { return (m_res ? (unsigned int)mysql_num_rows(m_res) : 0); }
        
        ///��ȡ�ֶθ���
        unsigned int GetFieldCount() { return (m_res ? (unsigned int)mysql_num_fields(m_res) : 0); }
        
        
        ///��ȡ��һ����¼��
        bool GetRecord()
        {
            if (m_res == NULL)
                return false;
            m_row = mysql_fetch_row(m_res);
            return (m_row != NULL);
        }

		///��ȡ��һ����¼��
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

        ///��ȡ��ǰ��¼����ĳһ���ֶε�ֵ
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
@brief ��װMySQL������
*
*	��װMySQL�����ӣ������ɶ���߳�ͬʱ����������ʵ��
*/

    class CMySQLConnection
    {
    public:
        ///����ExecuteEx�ķ���ֵ
        typedef enum {Failed, NoRecordSet, HasRecordSet} EExecResult;

        ///���캯��
        CMySQLConnection(void);

        ///��������
        ~CMySQLConnection(void);

        ///����������Ϣ
        void SetConnectionString(const char* pszServerAddr, int nServerPort,
            const char* pszDBName, const char* pszUserID, const char* pszPassword,const char *pConFormat);

        ///������
        bool Connect(const char* pszServerAddr, int nServerPort,
            const char* pszDBName, const char* pszUserID, const char* pszPassword,
			const char *pConFormat,string &strerror);

        ///�����ӣ�������������Ϣ�����
        bool Connect( string &strerror );

        ///�ر�����
        void Close();

        ///�Ƿ�������
        bool IsConnected();

        ///ת���ַ�����ʹ֮������MYSQL��ѯ
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

        ///�����ܵ����һ��UPDATE��DELETE��INSERT��ѯӰ��(�仯)������
        unsigned int GetAffectedRows() { return (unsigned int)mysql_affected_rows(m_pSqlCon); }

        ///ת���ַ�����ʹ֮������MYSQL��ѯ
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

        ///ת���ַ�����ʹ֮������MYSQL��ѯ����Ҫ����ת��2��������
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

        ///�޲��ִ��SQL��䣬ͨ������ֵ�жϾ���Ч��
        int  ExecuteEx(const char* pszSQL, CMySQLRecordSet& rcdSet);
        ///�޲��ִ��SQL��䣬ͨ������ֵ�жϾ���Ч��(֧�ֶ����ƴ�������)
        int  ExecuteEx(const char* pszSQL, int nLength,CMySQLRecordSet& rcdSet);

        ///ִ��һ��SQL��䣬�����ؼ�¼��
        bool Execute(const char* pszSQL);

        ///ִ��һ��SQL��䣬���ؼ�¼��
        bool Execute(const char* pszSQL, CMySQLRecordSet& rcdSet);

        ///��ȡ��һ�δ����
        unsigned int GetLastError()
        {
            if(m_pSqlCon!=NULL)
            {
                return (unsigned int)mysql_errno(m_pSqlCon);
            }
            return  0;
        }

        ///��ȡ��һ�δ�������
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
