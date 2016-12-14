/********************************************************************
	created:	28:5:2010   15:33
	filename: 	f:\SGS_2\Server\log\//InterfaceLog.h
	author:		Zhanghongtao
	
	purpose:	log interface
*********************************************************************/
#pragma once
#include "../../PeonyNet/include/Xbuildconfig_NiceNet.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;

using namespace peony::net;

extern INiceLog      *g_pAppLog;
extern void gfunNetLog_AppLog(int LogG, stringstream &oStr );

#define APPLOG_REALTIMESAVE()\
	do{\
		PEYONE_REALTIMESAVE_LOG( g_pAppLog );\
		INiceNet::Instance().SaveLog();\
	} while (0)

#define APPLOG_TOLOGSS(LOGINFO,LogG)\
	do{\
		try{\
			stringstream strStream;\
			strStream<<";"<<LOGINFO;\
			gfunNetLog_AppLog( LogG,strStream );\
		}catch(...){}\
	} while (0)

#define APPLOG_FATAL(LOGINFO)		do{PEYONE_LOG_MSG(g_pAppLog,LOGINFO,Log_Fatal );APPLOG_REALTIMESAVE();}while(0)
#define APPLOG_ERROR(LOGINFO)		do{PEYONE_LOG_MSG(g_pAppLog,LOGINFO,Log_Error );APPLOG_TOLOGSS(LOGINFO,Log_Error);}while(0)
#define APPLOG_WARNING(LOGINFO)		do{PEYONE_LOG_MSG(g_pAppLog,LOGINFO,Log_Warning );APPLOG_TOLOGSS(LOGINFO,Log_Warning);}while(0)
#define APPLOG_NORMAL(LOGINFO)		do{PEYONE_LOG_MSG(g_pAppLog,LOGINFO,Log_Normal );APPLOG_TOLOGSS(LOGINFO,Log_Normal);}while(0)
#define APPLOG_DEBUG(LOGINFO)		do{PEYONE_LOG_MSG(g_pAppLog,LOGINFO,Log_DebugOther );APPLOG_TOLOGSS(LOGINFO,Log_DebugOther);}while(0)
#define APPLOG_DEBUGCMD(LOGINFO)	do{PEYONE_LOG_MSG(g_pAppLog,LOGINFO,Log_DebugCmd );APPLOG_TOLOGSS(LOGINFO,Log_DebugCmd);}while(0)
#define APPLOG_SYSINFO(LOGINFO)	    do{PEYONE_LOG_MSG(g_pAppLog,LOGINFO,Log_SysNormalInfo );APPLOG_TOLOGSS(LOGINFO,Log_SysNormalInfo);}while(0)


#define APPLOG_SHOWMSGID(INFO, P, L)\
	do{\
		string OPCode = boost::str( boost::format("  OPCode=%d,msglen %d=%d, ")%P->OPCode%P->Len%L );\
		APPLOG_SYSINFO(INFO<<OPCode);\
	}while (0)


//三国杀逻辑层异常处理,判断宏定义
#define SGS_TRY_BEGIN        int ihelp_signal_try = 0; try{ ihelp_signal_try = 1
#define SGS_TRY_SIGNAL       ++ihelp_signal_try

#define SGS_TRY_0END         }catch(...){ APPLOG_ERROR("[这个函数发生了异常!] ihelp_signal_try="<<ihelp_signal_try<<FUN_FILE_LINE); }\
	                         ++ihelp_signal_try;

#define SGS_TRY_1END(PMSG)   }catch(...){ APPLOG_ERROR("[这个函数发生了异常!] ihelp_signal_try="<<ihelp_signal_try<<";OPCode="<<PMSG->OPCode<<FUN_FILE_LINE); }\
							 ++ihelp_signal_try;

#define SGS_TRY_2END(AA)     }catch(...){ APPLOG_ERROR("[这个函数发生了异常!] AA="<<AA<<FUN_FILE_LINE); }\
						 	 ++ihelp_signal_try;
