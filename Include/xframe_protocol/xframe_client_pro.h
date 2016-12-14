#ifndef xframe_client_pro_h__
#define xframe_client_pro_h__
#include <memory.h>
#include "../Common/xframe_types.h"


const unsigned int   sc_accountlen	    = 32; 		        //账号的长度
const unsigned int   sc_rolelen	        = 32; 		        //角色的长度
const unsigned int   sc_digitlen        = 32; 		        //数字帐号长度
const unsigned int   sc_pwmaxlen	    = 32; 		        //密码的长度
const unsigned int   sc_ipmaxlen	    = 16; 		        //ip的长度

enum OPCodeXFrameClient
{
	//********************************************************************
	//	purpose: client
	//*********************************************************************
	XFRAME_CLIENT_PROTOCOL_BEGIN =10100,   //协议分段------------------------------------------------------
    CC_GT_HELLOWORLD_REQ,                  //hello测试协议
	CC_GT_HELLOWORLD_REP,

	//gm指令
	C_GM_CMD_REQ,                          //[client]客户端的gm命令请求
	CC_USER_OPENCLOSE_GM_NTF,              //[client]打开或者关闭gm权限

	//通用消息，如果是相同类型的需求，请不要在定新协议
	MSG_HEART_ALIVE_REQ,                   //[client]心跳消息请求
	MSG_HEART_ALIVE_REP,                   //[client]心跳消息请求
	SS_C_MESSAGEBOX_YESNO_NTF,             //[client]服务器通知客户端一般消息
	C_SS_LOGMESSAGE_NTF,                   //[GT]ztylib netlog 一般消息


	//踢用户下线
	NOTICE_CLIENT_KICKYOU_NTF,             //[client]通知客户端，你被踢下线了[client]

	//App日志观察期协议 GmToolLog
	GMLOGTOOL_ORDER_APPLOG_REQ,			   //[GmAppLogToolView] 请求定制log
	GMLOGTOOL_ORDER_APPLOG_REP,

	//一些不需要处理的key 10112
	CXSXS_GETXKEY_REQ, 
	CXSXS_GETXKEY_REP,

	SS_LOGS_NETLOG_SAVE,

	SS_C_CONTROLJNINETLOG_CMD,				//控制jni层的log输出
	SS_C_CONTROLJNINETLOG_NTF,				//jni层的log信息
	

};

static const unsigned int  sc_invalid_user_temp_id		= 0;		   //无效的用户临时ID
static const unsigned int  sc_max_user_temp_id		    = 0XFA56EA00;  //4200000000;  //最大的户临时ID


//登录类型
enum emlogintype
{
	logintype_notknow=1,
	logintype_zty_android,
	logintype_zty_ios,
	logintype_zty_gmtool,
	logintype_zty_robot,

	logintype_count,
};

//不同的来源 包括客户端 手机
enum emloginfrom
{
	loginfrom_snda_bianfeng_sanguosha =0,
	loginfrom_count =1,
};

//踢用户下线愿意
enum em_kickuser_why
{
	kickkind_begin_kind=1,

	//踢单个用户类型
	kickkind_relogin_othergt,                   //用户重复登录
	kickkind_loginok_kick,                      //用户登录成功了，踢出不在相关服务器上的用户
	kickkind_dbs_loadbaseinfo_fail,             //dbs上加载用户基本数据的时候失败

	kickkind_gm,								//gm
	kickkind_finishoffline_iscs,                //用户在cs上完成了下线

	//自己的心跳超时
	kickkind_client_heartchecktimeout,          //client和gt心跳超时  
	kickkind_dbs_selfhearttimeout,              //dbs

	//下面的是批量踢用户
	kickkind_cs_system_kickalluser,				//系统维护
	kickkind_cs_theserver_disconnectcs,         //这个服务器和cs的连接断开

	kickkind_gw_youclass_bydel,					//班级被删除了

	kickkind_useroffline_end=200,
};


#pragma pack(push)
#pragma pack(1)
const unsigned short  sc_packetmsg_checkcode = 21301;
struct PacketBase
{
	PacketBase(unsigned int opcode, unsigned int len):OPCode(opcode),Len(len),user_id(sc_invalid_user_temp_id){CheckCode=sc_packetmsg_checkcode;BackID=0;}
	unsigned int    OPCode;          //操作类别  
	unsigned int    Len;             //整个消息包长度
	unsigned int    user_id;         //modify by zht
	unsigned short  CheckCode;       //校验吗21301;
	unsigned short  BackID;          //备用;
};

//hello测试协议
struct TCcGtHelloWorldReq : public PacketBase
{
	struct TInTestBody
	{
		short           test_short;
		unsigned short  test_ushort;
		int				test_int;
		unsigned int	test_uint;
		float           test_float;
		char            test_char;            //8位有符号整数
		unsigned char   test_uchar;           //8位无符号整数
		char            test_array[20];       //字符串

		unsigned int    test_strearm_len;     //变长的流
		unsigned int    test_ssarray_count;   //变长数组的个数

		char            test_strearm[128];    //[128>=test_strearm_len]
		char            test_ssarray[10][32]; //[10>=test_ssarray_count]

		/****************************************************************************************
		//说明:上面数组的定义定价于下面这样写，写成二维数组是为了方便程序的写，这个数组是变长的,
		//读的时候需要根据test_array_count的值来读取下面数组的实际长度，否则会造成溢出的问题.
		//如果test_array_count = 1, test_array[1][32],数据的长度是32 
		//如果test_array_count = 2, test_array[2][32],数据的长度是64 
		//char          test_array_count[320]; //[10>=test_array_count]
		*****************************************************************************************/

		//另外请注意这是两个变长的东西,test_ssarray数据开始的位置和前面的有关系
	};

	TInTestBody  body;
public:
	TCcGtHelloWorldReq() : PacketBase(CC_GT_HELLOWORLD_REQ, sizeof(TCcGtHelloWorldReq))
	{
	}
};
struct TCcGtHelloWorldRep : public PacketBase
{
	TCcGtHelloWorldReq::TInTestBody body;
public:
	TCcGtHelloWorldRep() : PacketBase(CC_GT_HELLOWORLD_REP, sizeof(TCcGtHelloWorldRep))
	{
	}
};


//客户端上来的GM指令
struct TCGmCmdReq : public PacketBase
{
	enum emGmCmdFrom
	{
		emGmFrom_Client       = 1,
		emGmFrom_SupperServer,
	};

	char gmcmd[256];
public:
	TCGmCmdReq() : PacketBase(C_GM_CMD_REQ, sizeof(TCGmCmdReq))
	{
		memset(gmcmd,0,sizeof(gmcmd) );
	}
};

//打开或者关闭gm权限
struct CCUserOpenCloseGmNtf : public PacketBase
{
	char           account[48]; 
	unsigned int   grade;
	bool           isopen;
public:
	CCUserOpenCloseGmNtf() : PacketBase(CC_USER_OPENCLOSE_GM_NTF, sizeof(CCUserOpenCloseGmNtf))
	{
		grade	= 0;
		isopen	= false;
		memset(account,0,sizeof(account) );
	}
};

//服务器通知客户端弹出对话框，让用户选择yes，no
struct SSToCMessageboxYesNoNtf : public PacketBase
{
	//选择结果
	enum emresult
	{
		MB_YES = 1,
		MB_NO,
	};

	//具体类型
	enum em_ss_apply
	{
		emsgbox_type_reminder=1, //提示信息
		emsgbox_type_enterpage,  //进入大厅
		emsgbox_type_debug,      //调试信息,现实在综合中
		emsgbox_type_GM,         //调试信息,现实在综合中,
		emsgbox_type_donate,     //赠送提示,有人赠送了你物品,

	};

	em_ss_apply    apply_type;
	unsigned short message_len;
	char           msginfo[512]; //[512>=message_len]最大消息长度
	SSToCMessageboxYesNoNtf():PacketBase(SS_C_MESSAGEBOX_YESNO_NTF, sizeof(SSToCMessageboxYesNoNtf))
	{
		memset( msginfo,0,sizeof(msginfo) );
	}
};

//[GT]ztylib netlog 一般消息
struct CToSSLogMessageNtf : public PacketBase
{//C_SS_LOGMESSAGE_NTF 10108
	unsigned int   appid_actid;  //20101101 = 20_101_101,(appid,appversion,appactiveid)
	int            logGrade;
	int            ibackpm;
	unsigned short message_len;
	char           msginfo[1000]; //[512>=message_len]最大消息长度
	CToSSLogMessageNtf():PacketBase(C_SS_LOGMESSAGE_NTF, sizeof(CToSSLogMessageNtf))
	{
		message_len = 0;
		memset( msginfo,0,sizeof(msginfo) );
	}
};
enum EmCcJniNetLogOp
{
	ECJniNetL_NoDef = 0,
	ECJniNetL_OpenNetLog,	//打开网络日志，jni  m_isOpenNet
	ECJniNetL_CloseNetLog,  //

	ECJniNetL_OpenErrLog,	//打开网络日志，jni  m_isOpenErrLog
	ECJniNetL_CloseErrLog,	//打开网络日志，jni  m_isOpenErrLog

	ECJniNetL_ClearAllKey,
	ECJniNetL_DelKey,
	ECJniNetL_AddKey,
};
struct CControlJniNetLogCmd : public PacketBase
{//SS_C_CONTROLJNINETLOG_CMD 10115
	EmCcJniNetLogOp  emOpType;
	char             chkey[40];

	unsigned int     gm_gtServerID;
	unsigned int     gm_gtConid;   //gm的客户端连接ID
	CControlJniNetLogCmd():PacketBase(SS_C_CONTROLJNINETLOG_CMD, sizeof(CControlJniNetLogCmd))
	{
		gm_gtServerID = 0;
		emOpType = ECJniNetL_NoDef;
		memset( chkey,0,sizeof(chkey) );
	}
};
//jni层的log信息
struct CControlJniNetLogNtf : public PacketBase
{//10116
	unsigned int     gm_gtServerID;
	unsigned int     gm_gtConid;   //gm的客户端连接ID

	EmCcJniNetLogOp  emOpType;
	char             JniInfo[256];
	CControlJniNetLogNtf():PacketBase(SS_C_CONTROLJNINETLOG_NTF, sizeof(CControlJniNetLogNtf))
	{
		gm_gtServerID = 0;
		emOpType = ECJniNetL_NoDef;
		memset( JniInfo,0,sizeof(JniInfo) );
	}
};

//心跳消息请求
struct MsgHeartAliveReq : public PacketBase
{
	unsigned int myindex; //表示客户端和gt直接的网络是否已经断开，服务器直接的
	MsgHeartAliveReq():PacketBase(MSG_HEART_ALIVE_REQ, sizeof(MsgHeartAliveReq))
	{
		myindex	= 0;
	}
};
struct MsgHeartAliveRep : public PacketBase
{
	unsigned int  ucurtime;
	unsigned int  myindex;
	MsgHeartAliveRep():PacketBase(MSG_HEART_ALIVE_REP, sizeof(MsgHeartAliveRep))
	{
		myindex  = 0;
		ucurtime = 0;
	}
};

//通知客户端，你被踢下线了[client]
struct MsgNoticeClientKickYouNtf : public PacketBase
{
	unsigned int paramA;   //没有使用
	unsigned int paramB;   //没有使用

	//em_kickuser_why why;
	unsigned int why;
	MsgNoticeClientKickYouNtf():PacketBase(NOTICE_CLIENT_KICKYOU_NTF, sizeof(MsgNoticeClientKickYouNtf))
	{
		paramA = paramB = 0;
		why = kickkind_begin_kind;
	}
};

//add by db
struct Msg_Net_Log : public PacketBase
{ 
	unsigned int	opCode;
	unsigned int	opErrCode;
	unsigned short	opParamsLen;
	char			opParams[2048];

	// 
	enum { OpErrCodeNoDef = 0,OpErrCodeOK };
public:
	Msg_Net_Log() : PacketBase(SS_LOGS_NETLOG_SAVE, sizeof(Msg_Net_Log))
	{ 
		opCode = 0;
		opErrCode = OpErrCodeNoDef;
		opParamsLen = 0;
		memset(opParams,0,sizeof(opParams));
	}
};

//[GmAppLogToolView] 请求定制log
struct GmLogToolOrderAppLogReq : public PacketBase
{
	char			account[60];
	char			userDigital[20];  //数字ID
	char            logkey[32];
	int				iAppID;
	unsigned int	iOrderAppLogGrade;
    unsigned int    ireqmarked;
	GmLogToolOrderAppLogReq():PacketBase(GMLOGTOOL_ORDER_APPLOG_REQ, sizeof(GmLogToolOrderAppLogReq))
	{
		iAppID				= 0;
		iOrderAppLogGrade	= 0;
		ireqmarked          = 0;
		memset(logkey,     0, sizeof(logkey) );
		memset(account,    0, sizeof(account) );
		memset(userDigital,0, sizeof(userDigital) );
	}
};

struct GmLogToolOrderAppLogRep : public PacketBase
{
	enum emResult{
		emResult_NoDef = 0,
		emResult_Ok,
		emResult_UserOffLine,
		emResult_AppTooBig,

	};
	unsigned int    ireqmarked;
	emResult		result;
	GmLogToolOrderAppLogRep():PacketBase(GMLOGTOOL_ORDER_APPLOG_REP, sizeof(GmLogToolOrderAppLogRep))
	{
		result = emResult_NoDef;
	}
};


#pragma pack(pop)
#endif // xframe_client_pro_h__