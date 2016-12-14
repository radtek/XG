#ifndef xframe_server_pro_h__
#define xframe_server_pro_h__
#include "./xframe_client_pro.h"

const unsigned int  sc_invalid_server_id         = 0;					//无效的server_id
const unsigned int  sc_invalid_msgchannelid      = 0;					//无效的频道号
const unsigned int  sc_invalid_tempbufobj_id     = 0;                   //无效的临时缓冲区id

enum EServerNetType
{ 
	eErrorServer    = 4000,
	eCenterServer,
	eLogServer,
	eXhLoginServer,
	eXhBookshelfServer,
	eZtyTransit,			//中转服务器,连接cs，dbs,web支持
	eGateway,
	eLoginServer, 
	eMySqlServer,
	eAppGuWenServer,		//古文刷题王
	ePbServer,
	eGtPreSs,
	eSchPreSs,
	eXhTsGroupSs,			//师生关系服务器
	eZTKTServer,            //智通课堂服务器
	eXhRankServer,          //排行榜服务器

	eMonitorServer,

	eDistrFrameCf	= 4020,
	eDistrFrameGt,
	eDistrFrameSs,

	eClient         = 4030,	//客户端

	eNotServer	    = 4999, 
};

enum OPCodeXFrameServer
{
	//服务器 <---------> 服务器
	XFRAME_SERVER_PROTOCOL_BEGIN = 10000,  //协议分段------------------------------------------------------

	//服务器之间的链接管理
	SELF_CS_REPORTNETTYPE_REQ,             //服务器自己到中心服务器注册自己的服务类型
	CS_NOTICE_NEWSERVERSTARTUP,            //cs通知别的服务器有新的服务器启动
	SS_SS_MYSTARTOK_NTF,                   //通知别的服务器我启动好了,可以处理逻辑数据了
	SS_CS_SELF_FAULT_NTF,                  //向cs报告我的故障，我该如何处理
	CS_SS_SGS_SERVER_SHUTDOWN_CMD,         //gm指令关闭服务器通知
	SS_SS_MYUSING_SERVERID_NTF,            //服务器更新了自己的网络链接id
	SERVER_TOSERVER_ALIVE_SELFSTRESS_RPT,  //服务器向中心服务器报告自己的压力参数

	//同道消息
	SS_GT_MSGCHANNEL_CMD,                  //[client]通知gt广播频道信息
	SS_GT_USER_INTO_OUT_CHANNEL_CMD,       //[client]用户进或出一个信道
	SS_GT_CHANNEL_OPERATE_REQ,             //[sc]请求操作信道，创建，删除
	SS_GT_CHANNEL_OPERATE_REP,             //

	//逻辑应用
	SERVER_SERVER_GROUPMSG_SYS,            //服务器直接的群发消息
	CLIENT_MSGBOXTOUSER_BYCSTRSFER,        //服务器请求cs把这个消息msgbox给制定用户
	SS_SS_REBOUND_REQ,                     //反弹消息
	SS_KICK_USER_OFFLINE_REQ ,			   //服务器请求踢用户下线
	CS_ENFORCE_KICK_USER_NTF,              //中心服务器命令踢用户下线 
	CS_TOSS_MYTIME_NTF,                    //中心通知别的服务器我的时间
	SS_WEBSERVER_TEST_REQ,                 //Webserver 测试消息 
	SS_WEBSERVER_TEST_REP,

	XS_TRS_XS_POSTMSG_REQ,				   //中转服务器中转
	XS_TRS_XS_POSTMSG_REP,           

	CS_GTCON_POSTMSG_REQ,				   //CS命令GT把个消息发送给这个连接

};

#pragma pack(push)
#pragma pack(1)


struct Msg_CS_C_ReportNetType : public PacketBase
{
	unsigned int   wServerNetType; 
	unsigned int   wServerIndex; 
	char           strVer[16]; 
	char           strAddress[16]; 
	unsigned short nListenPort;
	unsigned char  btGroup;
	unsigned char  btState;
public:
	Msg_CS_C_ReportNetType() : PacketBase(SELF_CS_REPORTNETTYPE_REQ, sizeof(Msg_CS_C_ReportNetType))
	{
		btGroup		= 0;
		btState		= 0;
		memset(strVer,0,sizeof(strVer) );
	}
};


struct Msg_CS_Notice_NewServerStartUp : public PacketBase
{
	static const unsigned short sc_max_size_item = 20;
	struct ItemServer
	{
		unsigned int   wServerNetType; 
		unsigned int   wServerID; 
		char           listenip[16]; 
		unsigned int   listenport;
	};

	unsigned int     item_count;
	ItemServer       itemarray[sc_max_size_item];
public:
	Msg_CS_Notice_NewServerStartUp() : PacketBase(CS_NOTICE_NEWSERVERSTARTUP, 0)
	{
		item_count = 0;
		Len = sizeof(PacketBase)+sizeof(item_count);
	}
};

struct SsSsMyStartOkNtf : public PacketBase
{
	enum emEventType
	{
		type_start_ok = 1,         //启动成功，可以接收逻辑业务
	};

	unsigned int     myserverid;  //我的id
	EServerNetType   mytype;      //我的服务器类型
	emEventType      eventtype;   //事件类型

public:
	SsSsMyStartOkNtf() : PacketBase(SS_SS_MYSTARTOK_NTF, sizeof(SsSsMyStartOkNtf))
	{
	}
};
/********************************************************************
created:	2010-9-10   16:44 author: Zhanghongtao
purpose:	向cs报告我的故障, cs如果需要我处理会返回消息 CSSS_FaultDealwithNtf
*********************************************************************/
struct SSCS_SelfFaultNtf : public PacketBase
{
	enum emFaultKind
	{
		fault_noinit = 0,
		fault_self_disconnect_otherserver = 1,       //我和一个服务器断开了
		fault_self_timeout_otherserver,              //服务器超时
	};

	union FaultData
	{
		struct t_disconnect_otherserver
		{
			unsigned int  otherserver_serverid;  //和我断开的serverid

		} discon_otherserver;
	};

public:
	emFaultKind             fault_kind;
	FaultData               data;
	unsigned int            fault_id;    //故障编号，用户需找
	SSCS_SelfFaultNtf():PacketBase(SS_CS_SELF_FAULT_NTF, sizeof(SSCS_SelfFaultNtf) )
	{
		fault_kind =fault_noinit;
		memset(&data,0,sizeof(FaultData) );
	}
};

//关闭服务器通知
struct CsSsSgsServerShutdownCmd : public PacketBase
{
	enum emCloseWhy
	{
		why_sys_restart = 1,         //系统重新启动, param秒后系统将重新启动
		why_yourserverid_exist,      //你的serverid和别的服务器重复
	};
	emCloseWhy		why;
	unsigned int    param;
	char            desc[64];
public:
	CsSsSgsServerShutdownCmd() : PacketBase(CS_SS_SGS_SERVER_SHUTDOWN_CMD, sizeof(CsSsSgsServerShutdownCmd))
	{
		param = 0;
		memset(desc,0,sizeof(desc) );
	}
};

//接收到消息的服务器更新自己的服务器id
struct SSSS_MeUsingServeridNtf : public PacketBase
{
	EServerNetType  update_servertype;  //故障服务器类型
	unsigned int    update_serverid;
public:
	SSSS_MeUsingServeridNtf():PacketBase(SS_SS_MYUSING_SERVERID_NTF, sizeof(SSSS_MeUsingServeridNtf) )
	{
		update_servertype = eNotServer;
		update_serverid	  = sc_invalid_server_id;
	}
};

/************************************************************************
CLinker服务器向自己连接的服务器报告自己的各种参数，
如果是中心服务器收到这个包，参数self_stress_parameter对cs比较重要，
当让这个包还有心跳消息的作用，
*************************************************************************/
struct MsgServerToServerSelfSysInfoRpt : public PacketBase
{
	unsigned int    self_stress_parameter;  //自己的压力参数,现在是人数,gt,gs,dbs
	unsigned int    self_alive_id;          //自己的心跳id,发送消息时候的时间
	unsigned int    self_app_startup_time;  //自己进程启动的时间
public:
	MsgServerToServerSelfSysInfoRpt() : PacketBase(SERVER_TOSERVER_ALIVE_SELFSTRESS_RPT, sizeof(MsgServerToServerSelfSysInfoRpt))
	{
		self_stress_parameter = 0;
		self_alive_id         = 0;
	}
};

//请求操作信道，创建，删除
//信道大类型
enum EmMsgChanType
{
	EmMsgCType_NoDef = 0,
	EmMsgCType_AllUser,
	EmMsgCType_School,
	EmMsgCType_XUnit,
	EmMsgCType_Class,
};
struct SsGtChannelOperateReq : public PacketBase
{
	enum emOperType
	{
		optype_nodef  = 0,
		optype_create = 1,
		optype_delete,
		optype_modifyparam,   //修改参数
	};
	emOperType     opertype;
	EmMsgChanType  chanType;
	unsigned int   channelid;
	char           desc[32];
	unsigned int   hashkey;   //信道的hashkey
public:
	SsGtChannelOperateReq():PacketBase(SS_GT_CHANNEL_OPERATE_REQ, sizeof(SsGtChannelOperateReq) )
	{
		opertype  = optype_nodef;
		channelid = sc_invalid_msgchannelid;
		hashkey   = 0;
		memset(desc,0,sizeof(desc));
	}
};
struct SsGtChannelOperateRep : public PacketBase
{
	enum emOperResult
	{
		opresult_ok = 1,
		opresult_fail,
	};

	emOperResult                          result;
	SsGtChannelOperateReq::emOperType     opertype;
	unsigned int                          channelid;
public:
	SsGtChannelOperateRep():PacketBase(SS_GT_CHANNEL_OPERATE_REP, sizeof(SsGtChannelOperateRep) )
	{
		//result    = 0;
		//opertype  = 0;
		channelid = sc_invalid_msgchannelid;
	}
};
struct SsGtMsgchannelCmd : public PacketBase
{
	EmMsgChanType  cType;
	unsigned int   channelid;
	unsigned int   msgbodylen;
	char           msgbody[1024];
public:
	SsGtMsgchannelCmd():PacketBase(SS_GT_MSGCHANNEL_CMD, sizeof(SsGtMsgchannelCmd) )
	{
		channelid  = 0;
		msgbodylen = 0;
	}
};
struct SsGtUserIntoOutChannelCmd : public PacketBase
{
	unsigned int   into_channelid;
	unsigned int   out_channelid;
public:
	SsGtUserIntoOutChannelCmd():PacketBase(SS_GT_USER_INTO_OUT_CHANNEL_CMD, sizeof(SsGtUserIntoOutChannelCmd) )
	{
		into_channelid = sc_invalid_msgchannelid;
		out_channelid  = sc_invalid_msgchannelid;
	}
};

//逻辑应用
//逻辑应用
//逻辑应用
//gt服务器请求cs把这个消息msgbox给制定用户,这个消息没有响应消息
struct ServerServerGroupMsgSys : public PacketBase
{
	static const unsigned short   sc_this_pack_maxlen    = 1024*10;
	static const unsigned short   sc_this_min_usercount  = 25;

	enum emGroupKind
	{
		groupmsg_kind_alluser_inserver    =1,  //目标服务器上的所有用户
		groupmsg_kind_partuser,                //部分用户，userlist的用户，用户ID
	};
public:
	emGroupKind		        groupmsg_kind;
	unsigned int            msg_len;
	unsigned int            usercount;
	//msg
	//userlist
	ServerServerGroupMsgSys():PacketBase(SERVER_SERVER_GROUPMSG_SYS, 0)
	{
	}
};


struct MessageboxToUserByCsTrasferReq : public PacketBase
{
	unsigned int	touser_temp_id;				//接收msgbox消息的用户
	char			account_or_nickname[48];	//昵称或者账户
	unsigned char   msgtype;                    //SSToCMessageboxYesNoNtf::em_ss_apply
	unsigned char	msginfolen;
	char			msginfo[256];
public:
	MessageboxToUserByCsTrasferReq():PacketBase(CLIENT_MSGBOXTOUSER_BYCSTRSFER, sizeof(MessageboxToUserByCsTrasferReq) )
	{
		touser_temp_id = sc_invalid_user_temp_id;
		memset(account_or_nickname,0,sizeof(account_or_nickname) );
		memset(msginfo,0,sizeof(msginfo) );
	}
};

// struct SsDbsNetLogSaveReq : public PacketBase
// {
// 	unsigned char	bigtype;		//大类型，决定写到那个log表中
// 	unsigned int	littype;		//小类型，一个表中的类型
// 	unsigned int	DetailsID;      //自己定义
// 	unsigned int    serverid;       //服务器ID
// 	unsigned int    PAA;
// 	unsigned int    PAB;
// 	char            nickname[16];   //有可能为空，表示用户
// 	char            account[50];    //有可能为空，表示用户
// 	unsigned short  loginfolen;     //信息的长度
// 	char			loginfo[250];
// 	SsDbsNetLogSaveReq():PacketBase(SS_DBS_NETLOG_SAVE_REQ, sizeof(SsDbsNetLogSaveReq) )
// 	{
// 		bigtype   = 0;
// 		littype   = 0;
// 		DetailsID = 0;
// 		memset(&nickname, 0,sizeof(nickname) );
// 		memset(&loginfo,0,sizeof(loginfo) );
// 		memset(&account, 0,sizeof(account) );
// 	}
// };

struct TSsSsReboundReq : public PacketBase
{
	unsigned int ReBoundMsgLen;           //submsg
	char         ReBoundMsg[512];
	TSsSsReboundReq():PacketBase(SS_SS_REBOUND_REQ, sizeof(TSsSsReboundReq))
	{
		ReBoundMsgLen = 0;
		memset(&ReBoundMsg, 0, sizeof(ReBoundMsg));
	}
};
struct MsgKickUserOfflineReq : public PacketBase
{
public:
	em_kickuser_why					 why;
	int                              whyDetailed;

	//这些数据对不同的踢人类型，使用了不同的参数项
	unsigned int					 req_server_id;    //申请故障的服务器ID
	unsigned int					 blooey_server_id; //出现故障的服务器ID
	MsgKickUserOfflineReq():PacketBase(SS_KICK_USER_OFFLINE_REQ, sizeof(MsgKickUserOfflineReq))
	{
		why					= kickkind_begin_kind;
		whyDetailed         = 0;
		req_server_id		= 0;
		blooey_server_id	= sc_invalid_server_id;
	}
};
struct MsgCsKickUserCmd : public PacketBase
{
	char          account[sc_accountlen];
	unsigned int  why;                     //em_kickuser_why 为了方便
	int           whyDetailed;
	unsigned int  server_id;
	unsigned      newUser_dbsServerID;      //新上线的,使用的DBS
	unsigned      newUser_gtServerID;		//新上线的，使用的gt
	unsigned      newUser_uconid_toclient;  //新上线的，使用的连接gt ID
	MsgCsKickUserCmd():PacketBase(CS_ENFORCE_KICK_USER_NTF, sizeof(MsgCsKickUserCmd))
	{
		newUser_gtServerID		= 0;
		newUser_uconid_toclient = 0;
		newUser_dbsServerID     = 0;
		server_id				= 0;
		whyDetailed				= 0;
		why						= kickkind_begin_kind;
	}
};

//中心通知别的服务器我的时间
struct CsTossMytimeNtf : public PacketBase
{
	unsigned int   cs_cur_time;
public:
	CsTossMytimeNtf() : PacketBase(CS_TOSS_MYTIME_NTF, sizeof(CsTossMytimeNtf))
	{
	}
};

struct SsWebServerTestReq : public PacketBase
{
	unsigned int   reqindex;
	char           chdata[5000];
public:
	SsWebServerTestReq() : PacketBase(SS_WEBSERVER_TEST_REQ, sizeof(SsWebServerTestReq))
	{
		memset(chdata,0,sizeof(chdata) );
	}
};
struct SsWebServerTestRep : public PacketBase
{
	unsigned int   reqindex;
	char           chdata[5000];
public:
	SsWebServerTestRep() : PacketBase(SS_WEBSERVER_TEST_REP, sizeof(SsWebServerTestRep))
	{
		memset(chdata,0,sizeof(chdata) );
	}
};

//中转服务器中转
struct TXsTrsXsPostMsgReq : public PacketBase
{
	unsigned int    random_index;
	unsigned int	des_serverid;
	unsigned int    submsg_len;
	char			submsg[1024*40];
public:
	TXsTrsXsPostMsgReq() : PacketBase(XS_TRS_XS_POSTMSG_REQ, sizeof(TXsTrsXsPostMsgReq))
	{
		random_index  = 0;
		des_serverid  = 0;
		submsg_len    = 0;
		memset( submsg,0,sizeof(submsg) );
	}
};
enum emTrsXsResult
{
	TrsXsR_NoDef              =0,
	TrsXsR_OK                 =1,
	TrsXsR_MeNotConDesServer,
	TrsXsR_SendFail,
};
struct TXsTrsXsPostMsgRep : public PacketBase
{
	emTrsXsResult   result;
	unsigned int    random_index;
	unsigned int	des_serverid;
	unsigned int    submsg_len;
	unsigned int    submsg_opcode;
public:
	TXsTrsXsPostMsgRep() : PacketBase(XS_TRS_XS_POSTMSG_REP, sizeof(TXsTrsXsPostMsgRep))
	{
		result        = TrsXsR_NoDef;
		random_index  = 0;
		des_serverid  = 0;
		submsg_len    = 0;
		submsg_opcode = 0;
	}
};
//CS命令GT把个消息发送给这个连接
struct TCsGtConPostMsgReq : public PacketBase
{
	unsigned int	xs_serverid;    //发送服务器的ID
	unsigned int	gt_serverid;
	unsigned int    gt_cc_conid;	//客户端的连接ID
	unsigned int    submsg_len;
	char			submsg[1024*40];
public:
	TCsGtConPostMsgReq() : PacketBase(CS_GTCON_POSTMSG_REQ, sizeof(TCsGtConPostMsgReq) )
	{
		xs_serverid   = 0;
		gt_serverid   = 0;
		gt_cc_conid   = 0;
		submsg_len    = 0;
		memset( submsg,0,sizeof(submsg) );
	}
};

#pragma pack(pop)


#endif // xframe_server_pro_h__