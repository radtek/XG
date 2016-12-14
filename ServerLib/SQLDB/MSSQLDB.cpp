/** @file SQLDB.cpp @brief MSSQL��MySQL�ӿڷ�װ��ʵ��
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

#include "../include/MSSQLDB.h"


snda::CMSSQLConnection::CMSSQLConnection(void) : m_pDBConnection(NULL), m_dwErrCode(0)
{
    
}

snda::CMSSQLConnection::~CMSSQLConnection(void)
{
    if (IsConnected())
        Close();
}

BOOL snda::CMSSQLConnection::Connect(const char* pszConnectionString)
{
    SetConnectionString(pszConnectionString);
    return  Connect();
}


BOOL snda::CMSSQLConnection::Connect()
{
    if (m_strConnectionString.empty())
        return FALSE;
    try
    {
        if (m_pDBConnection == NULL)
        {
            if (FAILED(m_pDBConnection.CreateInstance(__uuidof(Connection))))
                return FALSE;  //�޷���ʼ��ADO����,�����MDAC
        }
        TESTHR(m_pDBConnection->Open(_bstr_t(m_strConnectionString.c_str()), "", "", -1));
    }
    catch(_com_error& e)
    {
        SetLastError(e);
        return FALSE; 
    }
    
    return TRUE;
}

BOOL snda::CMSSQLConnection::IsConnected()
{
    if (m_pDBConnection == NULL)
        return FALSE;
    return (m_pDBConnection->State == adStateOpen);
}

void snda::CMSSQLConnection::Close()
{
    if (m_pDBConnection != NULL && m_pDBConnection->State != adStateClosed)
    {
//        try
//        {
            m_pDBConnection->Close();
        //}
        //catch(...)
        //{
        //}
    }
}

BOOL snda::CMSSQLConnection::Execute(CMSSQLCommand& command)
{
    try
    {
        command.m_pCommand->ActiveConnection = m_pDBConnection;
        command.m_pCommand->Execute(NULL, NULL, command.m_pCommand->CommandType);
    }
    catch(_com_error& e)
    {
        SetLastError(e);
        WORD wError = e.WCode();
        if(wError == 0)
            Close();
        return FALSE;
    }
    return TRUE;
}

BOOL snda::CMSSQLConnection::Execute(CMSSQLCommand& command, CMSSQLRecordSet& pSet)
{
    try
    {
        command.m_pCommand->ActiveConnection = m_pDBConnection;
        pSet.m_pRecordSet = command.m_pCommand->Execute(NULL, NULL, command.m_pCommand->CommandType);
    }
    catch(_com_error& e)
    {
        SetLastError(e);
        if(e.WCode() == 0)
            Close();
        return FALSE;
    }
    return TRUE;
}

BOOL snda::CMSSQLConnection::Execute(char* pszSQL, CMSSQLRecordSet& pSet)
{
    try
    {
        pSet.m_pRecordSet = m_pDBConnection->Execute(_bstr_t(pszSQL), NULL, adCmdText);
    }
    catch(_com_error& e)
    {
        SetLastError(e);
        if(e.WCode() == 0)
            Close();
        return FALSE;
    }
    return TRUE;
}

BOOL snda::CMSSQLConnection::Execute( char* pszSQL)
{
    try
    {
        m_pDBConnection->Execute(_bstr_t(pszSQL), NULL, adCmdText);
    }
    catch(_com_error& e)
    {
        SetLastError(e);
        if(e.WCode() == 0)
            Close();
        return FALSE;
    }
    return TRUE;
}

