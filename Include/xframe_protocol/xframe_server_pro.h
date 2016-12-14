#ifndef xframe_server_pro_h__
#define xframe_server_pro_h__
#include "./xframe_client_pro.h"

const unsigned int  sc_invalid_server_id         = 0;					//��Ч��server_id
const unsigned int  sc_invalid_msgchannelid      = 0;					//��Ч��Ƶ����
const unsigned int  sc_invalid_tempbufobj_id     = 0;                   //��Ч����ʱ������id

enum EServerNetType
{ 
	eErrorServer    = 4000,
	eCenterServer,
	eLogServer,
	eXhLoginServer,
	eXhBookshelfServer,
	eZtyTransit,			//��ת������,����cs��dbs,web֧��
	eGateway,
	eLoginServer, 
	eMySqlServer,
	eAppGuWenServer,		//����ˢ����
	ePbServer,
	eGtPreSs,
	eSchPreSs,
	eXhTsGroupSs,			//ʦ����ϵ������
	eZTKTServer,            //��ͨ���÷�����
	eXhRankServer,          //���а������

	eMonitorServer,

	eDistrFrameCf	= 4020,
	eDistrFrameGt,
	eDistrFrameSs,

	eClient         = 4030,	//�ͻ���

	eNotServer	    = 4999, 
};

enum OPCodeXFrameServer
{
	//������ <---------> ������
	XFRAME_SERVER_PROTOCOL_BEGIN = 10000,  //Э��ֶ�------------------------------------------------------

	//������֮������ӹ���
	SELF_CS_REPORTNETTYPE_REQ,             //�������Լ������ķ�����ע���Լ��ķ�������
	CS_NOTICE_NEWSERVERSTARTUP,            //cs֪ͨ��ķ��������µķ���������
	SS_SS_MYSTARTOK_NTF,                   //֪ͨ��ķ���������������,���Դ����߼�������
	SS_CS_SELF_FAULT_NTF,                  //��cs�����ҵĹ��ϣ��Ҹ���δ���
	CS_SS_SGS_SERVER_SHUTDOWN_CMD,         //gmָ��رշ�����֪ͨ
	SS_SS_MYUSING_SERVERID_NTF,            //�������������Լ�����������id
	SERVER_TOSERVER_ALIVE_SELFSTRESS_RPT,  //�����������ķ����������Լ���ѹ������

	//ͬ����Ϣ
	SS_GT_MSGCHANNEL_CMD,                  //[client]֪ͨgt�㲥Ƶ����Ϣ
	SS_GT_USER_INTO_OUT_CHANNEL_CMD,       //[client]�û������һ���ŵ�
	SS_GT_CHANNEL_OPERATE_REQ,             //[sc]��������ŵ���������ɾ��
	SS_GT_CHANNEL_OPERATE_REP,             //

	//�߼�Ӧ��
	SERVER_SERVER_GROUPMSG_SYS,            //������ֱ�ӵ�Ⱥ����Ϣ
	CLIENT_MSGBOXTOUSER_BYCSTRSFER,        //����������cs�������Ϣmsgbox���ƶ��û�
	SS_SS_REBOUND_REQ,                     //������Ϣ
	SS_KICK_USER_OFFLINE_REQ ,			   //�������������û�����
	CS_ENFORCE_KICK_USER_NTF,              //���ķ������������û����� 
	CS_TOSS_MYTIME_NTF,                    //����֪ͨ��ķ������ҵ�ʱ��
	SS_WEBSERVER_TEST_REQ,                 //Webserver ������Ϣ 
	SS_WEBSERVER_TEST_REP,

	XS_TRS_XS_POSTMSG_REQ,				   //��ת��������ת
	XS_TRS_XS_POSTMSG_REP,           

	CS_GTCON_POSTMSG_REQ,				   //CS����GT�Ѹ���Ϣ���͸��������

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
		type_start_ok = 1,         //�����ɹ������Խ����߼�ҵ��
	};

	unsigned int     myserverid;  //�ҵ�id
	EServerNetType   mytype;      //�ҵķ���������
	emEventType      eventtype;   //�¼�����

public:
	SsSsMyStartOkNtf() : PacketBase(SS_SS_MYSTARTOK_NTF, sizeof(SsSsMyStartOkNtf))
	{
	}
};
/********************************************************************
created:	2010-9-10   16:44 author: Zhanghongtao
purpose:	��cs�����ҵĹ���, cs�����Ҫ�Ҵ���᷵����Ϣ CSSS_FaultDealwithNtf
*********************************************************************/
struct SSCS_SelfFaultNtf : public PacketBase
{
	enum emFaultKind
	{
		fault_noinit = 0,
		fault_self_disconnect_otherserver = 1,       //�Һ�һ���������Ͽ���
		fault_self_timeout_otherserver,              //��������ʱ
	};

	union FaultData
	{
		struct t_disconnect_otherserver
		{
			unsigned int  otherserver_serverid;  //���ҶϿ���serverid

		} discon_otherserver;
	};

public:
	emFaultKind             fault_kind;
	FaultData               data;
	unsigned int            fault_id;    //���ϱ�ţ��û�����
	SSCS_SelfFaultNtf():PacketBase(SS_CS_SELF_FAULT_NTF, sizeof(SSCS_SelfFaultNtf) )
	{
		fault_kind =fault_noinit;
		memset(&data,0,sizeof(FaultData) );
	}
};

//�رշ�����֪ͨ
struct CsSsSgsServerShutdownCmd : public PacketBase
{
	enum emCloseWhy
	{
		why_sys_restart = 1,         //ϵͳ��������, param���ϵͳ����������
		why_yourserverid_exist,      //���serverid�ͱ�ķ������ظ�
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

//���յ���Ϣ�ķ����������Լ��ķ�����id
struct SSSS_MeUsingServeridNtf : public PacketBase
{
	EServerNetType  update_servertype;  //���Ϸ���������
	unsigned int    update_serverid;
public:
	SSSS_MeUsingServeridNtf():PacketBase(SS_SS_MYUSING_SERVERID_NTF, sizeof(SSSS_MeUsingServeridNtf) )
	{
		update_servertype = eNotServer;
		update_serverid	  = sc_invalid_server_id;
	}
};

/************************************************************************
CLinker���������Լ����ӵķ����������Լ��ĸ��ֲ�����
��������ķ������յ������������self_stress_parameter��cs�Ƚ���Ҫ��
�������������������Ϣ�����ã�
*************************************************************************/
struct MsgServerToServerSelfSysInfoRpt : public PacketBase
{
	unsigned int    self_stress_parameter;  //�Լ���ѹ������,����������,gt,gs,dbs
	unsigned int    self_alive_id;          //�Լ�������id,������Ϣʱ���ʱ��
	unsigned int    self_app_startup_time;  //�Լ�����������ʱ��
public:
	MsgServerToServerSelfSysInfoRpt() : PacketBase(SERVER_TOSERVER_ALIVE_SELFSTRESS_RPT, sizeof(MsgServerToServerSelfSysInfoRpt))
	{
		self_stress_parameter = 0;
		self_alive_id         = 0;
	}
};

//��������ŵ���������ɾ��
//�ŵ�������
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
		optype_modifyparam,   //�޸Ĳ���
	};
	emOperType     opertype;
	EmMsgChanType  chanType;
	unsigned int   channelid;
	char           desc[32];
	unsigned int   hashkey;   //�ŵ���hashkey
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

//�߼�Ӧ��
//�߼�Ӧ��
//�߼�Ӧ��
//gt����������cs�������Ϣmsgbox���ƶ��û�,�����Ϣû����Ӧ��Ϣ
struct ServerServerGroupMsgSys : public PacketBase
{
	static const unsigned short   sc_this_pack_maxlen    = 1024*10;
	static const unsigned short   sc_this_min_usercount  = 25;

	enum emGroupKind
	{
		groupmsg_kind_alluser_inserver    =1,  //Ŀ��������ϵ������û�
		groupmsg_kind_partuser,                //�����û���userlist���û����û�ID
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
	unsigned int	touser_temp_id;				//����msgbox��Ϣ���û�
	char			account_or_nickname[48];	//�ǳƻ����˻�
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
// 	unsigned char	bigtype;		//�����ͣ�����д���Ǹ�log����
// 	unsigned int	littype;		//С���ͣ�һ�����е�����
// 	unsigned int	DetailsID;      //�Լ�����
// 	unsigned int    serverid;       //������ID
// 	unsigned int    PAA;
// 	unsigned int    PAB;
// 	char            nickname[16];   //�п���Ϊ�գ���ʾ�û�
// 	char            account[50];    //�п���Ϊ�գ���ʾ�û�
// 	unsigned short  loginfolen;     //��Ϣ�ĳ���
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

	//��Щ���ݶԲ�ͬ���������ͣ�ʹ���˲�ͬ�Ĳ�����
	unsigned int					 req_server_id;    //������ϵķ�����ID
	unsigned int					 blooey_server_id; //���ֹ��ϵķ�����ID
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
	unsigned int  why;                     //em_kickuser_why Ϊ�˷���
	int           whyDetailed;
	unsigned int  server_id;
	unsigned      newUser_dbsServerID;      //�����ߵ�,ʹ�õ�DBS
	unsigned      newUser_gtServerID;		//�����ߵģ�ʹ�õ�gt
	unsigned      newUser_uconid_toclient;  //�����ߵģ�ʹ�õ�����gt ID
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

//����֪ͨ��ķ������ҵ�ʱ��
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

//��ת��������ת
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
//CS����GT�Ѹ���Ϣ���͸��������
struct TCsGtConPostMsgReq : public PacketBase
{
	unsigned int	xs_serverid;    //���ͷ�������ID
	unsigned int	gt_serverid;
	unsigned int    gt_cc_conid;	//�ͻ��˵�����ID
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