/********************************************************************
	created:	30:6:2010   9:24
	filename: 	f:\game_server_v3\publib\Frame\netlog\interfacenetlog.h
	author:		Zhanghongtao
	
	purpose:	支持网络log模块
*********************************************************************/
#pragma once
#include "../../Include/Common/winflat.h"
 
#define NETLOG_DELIMITER "&\"@"
#define NETLOG_DELIMITER_LEN (sizeof(NETLOG_DELIMITER)-1)
 

struct NetLog { 
	enum {
		OPCODE_SYS_BEGIN = 1,
		OPCODE_SYS_LOG,
		OPCODE_SERVER_EXCEPTION,
		OPCODE_APPLOCALLOG,			//各自服务器本地的log

		OPCODE_CLIENT_ERRLOG=100,

		OPCODE_USER_BEGIN = 10000000,
		OPCODE_USER_LOG,
		OPCODE_USER_LOGIN,
		OPCODE_USER_LOGOUT
	};
	 
	NetLog(unsigned opCode,unsigned int opErrCode) 
		: started_(false),ended_(false),opCode_(opCode),opErrCode_(opErrCode) { 
	}
	~NetLog(void) { 
		log(opCode_,opErrCode_,oss_.str());
	}
	 
	template<typename T>
	NetLog& put(const T &t) { 
		if (!started_)
			started_ = true; 
		else  
			oss_ << NETLOG_DELIMITER; 

		oss_ << t;
		return *this;
	} 

	template<typename T>
	NetLog& operator << (const T &t) { 
		if (started_ && !ended_) {
			ended_ = true;
			oss_ << NETLOG_DELIMITER; 
		}
		oss_ << t;
		return *this;
	}  
private:

	static void log(unsigned opCode,unsigned int opErrCode,const string &paramString);

	bool started_;
	bool ended_;

	unsigned opCode_;
	unsigned opErrCode_;
	ostringstream oss_;
};

extern void gfunNetLog_AppLog(int LogG, string &strLog );
////////////////////////////////////////////////////////////////////////////

#define NETLOG_USER_LOGIN(opErrCode,userId,account,others) do {  NetLog(NetLog::OPCODE_USER_LOGIN,opErrCode).put(userId).put(account) << others; } while(0)
#define NETLOG_APPLOG( logGrade,others) do {  NetLog(NetLog::OPCODE_APPLOCALLOG,0).put(0).put("AppLog") << others; } while(0)

#define NETLOG_USER_LOGIN_USERID(opErrCode,userId,others) NETLOG_USER_LOGIN(opErrCode,userId,"",others)
#define NETLOG_USER_LOGIN_ACCOUNT(opErrCode,account,others) NETLOG_USER_LOGIN(opErrCode,0,account,others)

#define NETLOG_USER_LOGOUT(userId,others) do { NetLog(NetLog::OPCODE_USER_LOGOUT,0).put(userId) << others; } while(0)

#define NETLOG_SYS_EXCEPTION(opErrCode,others) do { \
	NetLog(NetLog::OPCODE_SERVER_EXCEPTION,opErrCode) << others<<FUN_FILE_LINE; \
} while(0)

#define NETLOG_CC_ERROR(opErrCode,userId,account,others) do {  NetLog(NetLog::OPCODE_CLIENT_ERRLOG,opErrCode).put(userId).put(account) << others; } while(0)
