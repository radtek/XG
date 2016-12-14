#ifndef xframe_client_pro_h__
#define xframe_client_pro_h__
#include <memory.h>
#include "../Common/xframe_types.h"


const unsigned int   sc_accountlen	    = 32; 		        //�˺ŵĳ���
const unsigned int   sc_rolelen	        = 32; 		        //��ɫ�ĳ���
const unsigned int   sc_digitlen        = 32; 		        //�����ʺų���
const unsigned int   sc_pwmaxlen	    = 32; 		        //����ĳ���
const unsigned int   sc_ipmaxlen	    = 16; 		        //ip�ĳ���

enum OPCodeXFrameClient
{
	//********************************************************************
	//	purpose: client
	//*********************************************************************
	XFRAME_CLIENT_PROTOCOL_BEGIN =10100,   //Э��ֶ�------------------------------------------------------
    CC_GT_HELLOWORLD_REQ,                  //hello����Э��
	CC_GT_HELLOWORLD_REP,

	//gmָ��
	C_GM_CMD_REQ,                          //[client]�ͻ��˵�gm��������
	CC_USER_OPENCLOSE_GM_NTF,              //[client]�򿪻��߹ر�gmȨ��

	//ͨ����Ϣ���������ͬ���͵������벻Ҫ�ڶ���Э��
	MSG_HEART_ALIVE_REQ,                   //[client]������Ϣ����
	MSG_HEART_ALIVE_REP,                   //[client]������Ϣ����
	SS_C_MESSAGEBOX_YESNO_NTF,             //[client]������֪ͨ�ͻ���һ����Ϣ
	C_SS_LOGMESSAGE_NTF,                   //[GT]ztylib netlog һ����Ϣ


	//���û�����
	NOTICE_CLIENT_KICKYOU_NTF,             //[client]֪ͨ�ͻ��ˣ��㱻��������[client]

	//App��־�۲���Э�� GmToolLog
	GMLOGTOOL_ORDER_APPLOG_REQ,			   //[GmAppLogToolView] ������log
	GMLOGTOOL_ORDER_APPLOG_REP,

	//һЩ����Ҫ�����key 10112
	CXSXS_GETXKEY_REQ, 
	CXSXS_GETXKEY_REP,

	SS_LOGS_NETLOG_SAVE,

	SS_C_CONTROLJNINETLOG_CMD,				//����jni���log���
	SS_C_CONTROLJNINETLOG_NTF,				//jni���log��Ϣ
	

};

static const unsigned int  sc_invalid_user_temp_id		= 0;		   //��Ч���û���ʱID
static const unsigned int  sc_max_user_temp_id		    = 0XFA56EA00;  //4200000000;  //���Ļ���ʱID


//��¼����
enum emlogintype
{
	logintype_notknow=1,
	logintype_zty_android,
	logintype_zty_ios,
	logintype_zty_gmtool,
	logintype_zty_robot,

	logintype_count,
};

//��ͬ����Դ �����ͻ��� �ֻ�
enum emloginfrom
{
	loginfrom_snda_bianfeng_sanguosha =0,
	loginfrom_count =1,
};

//���û�����Ը��
enum em_kickuser_why
{
	kickkind_begin_kind=1,

	//�ߵ����û�����
	kickkind_relogin_othergt,                   //�û��ظ���¼
	kickkind_loginok_kick,                      //�û���¼�ɹ��ˣ��߳�������ط������ϵ��û�
	kickkind_dbs_loadbaseinfo_fail,             //dbs�ϼ����û��������ݵ�ʱ��ʧ��

	kickkind_gm,								//gm
	kickkind_finishoffline_iscs,                //�û���cs�����������

	//�Լ���������ʱ
	kickkind_client_heartchecktimeout,          //client��gt������ʱ  
	kickkind_dbs_selfhearttimeout,              //dbs

	//��������������û�
	kickkind_cs_system_kickalluser,				//ϵͳά��
	kickkind_cs_theserver_disconnectcs,         //�����������cs�����ӶϿ�

	kickkind_gw_youclass_bydel,					//�༶��ɾ����

	kickkind_useroffline_end=200,
};


#pragma pack(push)
#pragma pack(1)
const unsigned short  sc_packetmsg_checkcode = 21301;
struct PacketBase
{
	PacketBase(unsigned int opcode, unsigned int len):OPCode(opcode),Len(len),user_id(sc_invalid_user_temp_id){CheckCode=sc_packetmsg_checkcode;BackID=0;}
	unsigned int    OPCode;          //�������  
	unsigned int    Len;             //������Ϣ������
	unsigned int    user_id;         //modify by zht
	unsigned short  CheckCode;       //У����21301;
	unsigned short  BackID;          //����;
};

//hello����Э��
struct TCcGtHelloWorldReq : public PacketBase
{
	struct TInTestBody
	{
		short           test_short;
		unsigned short  test_ushort;
		int				test_int;
		unsigned int	test_uint;
		float           test_float;
		char            test_char;            //8λ�з�������
		unsigned char   test_uchar;           //8λ�޷�������
		char            test_array[20];       //�ַ���

		unsigned int    test_strearm_len;     //�䳤����
		unsigned int    test_ssarray_count;   //�䳤����ĸ���

		char            test_strearm[128];    //[128>=test_strearm_len]
		char            test_ssarray[10][32]; //[10>=test_ssarray_count]

		/****************************************************************************************
		//˵��:��������Ķ��嶨������������д��д�ɶ�ά������Ϊ�˷�������д����������Ǳ䳤��,
		//����ʱ����Ҫ����test_array_count��ֵ����ȡ���������ʵ�ʳ��ȣ������������������.
		//���test_array_count = 1, test_array[1][32],���ݵĳ�����32 
		//���test_array_count = 2, test_array[2][32],���ݵĳ�����64 
		//char          test_array_count[320]; //[10>=test_array_count]
		*****************************************************************************************/

		//������ע�����������䳤�Ķ���,test_ssarray���ݿ�ʼ��λ�ú�ǰ����й�ϵ
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


//�ͻ���������GMָ��
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

//�򿪻��߹ر�gmȨ��
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

//������֪ͨ�ͻ��˵����Ի������û�ѡ��yes��no
struct SSToCMessageboxYesNoNtf : public PacketBase
{
	//ѡ����
	enum emresult
	{
		MB_YES = 1,
		MB_NO,
	};

	//��������
	enum em_ss_apply
	{
		emsgbox_type_reminder=1, //��ʾ��Ϣ
		emsgbox_type_enterpage,  //�������
		emsgbox_type_debug,      //������Ϣ,��ʵ���ۺ���
		emsgbox_type_GM,         //������Ϣ,��ʵ���ۺ���,
		emsgbox_type_donate,     //������ʾ,��������������Ʒ,

	};

	em_ss_apply    apply_type;
	unsigned short message_len;
	char           msginfo[512]; //[512>=message_len]�����Ϣ����
	SSToCMessageboxYesNoNtf():PacketBase(SS_C_MESSAGEBOX_YESNO_NTF, sizeof(SSToCMessageboxYesNoNtf))
	{
		memset( msginfo,0,sizeof(msginfo) );
	}
};

//[GT]ztylib netlog һ����Ϣ
struct CToSSLogMessageNtf : public PacketBase
{//C_SS_LOGMESSAGE_NTF 10108
	unsigned int   appid_actid;  //20101101 = 20_101_101,(appid,appversion,appactiveid)
	int            logGrade;
	int            ibackpm;
	unsigned short message_len;
	char           msginfo[1000]; //[512>=message_len]�����Ϣ����
	CToSSLogMessageNtf():PacketBase(C_SS_LOGMESSAGE_NTF, sizeof(CToSSLogMessageNtf))
	{
		message_len = 0;
		memset( msginfo,0,sizeof(msginfo) );
	}
};
enum EmCcJniNetLogOp
{
	ECJniNetL_NoDef = 0,
	ECJniNetL_OpenNetLog,	//��������־��jni  m_isOpenNet
	ECJniNetL_CloseNetLog,  //

	ECJniNetL_OpenErrLog,	//��������־��jni  m_isOpenErrLog
	ECJniNetL_CloseErrLog,	//��������־��jni  m_isOpenErrLog

	ECJniNetL_ClearAllKey,
	ECJniNetL_DelKey,
	ECJniNetL_AddKey,
};
struct CControlJniNetLogCmd : public PacketBase
{//SS_C_CONTROLJNINETLOG_CMD 10115
	EmCcJniNetLogOp  emOpType;
	char             chkey[40];

	unsigned int     gm_gtServerID;
	unsigned int     gm_gtConid;   //gm�Ŀͻ�������ID
	CControlJniNetLogCmd():PacketBase(SS_C_CONTROLJNINETLOG_CMD, sizeof(CControlJniNetLogCmd))
	{
		gm_gtServerID = 0;
		emOpType = ECJniNetL_NoDef;
		memset( chkey,0,sizeof(chkey) );
	}
};
//jni���log��Ϣ
struct CControlJniNetLogNtf : public PacketBase
{//10116
	unsigned int     gm_gtServerID;
	unsigned int     gm_gtConid;   //gm�Ŀͻ�������ID

	EmCcJniNetLogOp  emOpType;
	char             JniInfo[256];
	CControlJniNetLogNtf():PacketBase(SS_C_CONTROLJNINETLOG_NTF, sizeof(CControlJniNetLogNtf))
	{
		gm_gtServerID = 0;
		emOpType = ECJniNetL_NoDef;
		memset( JniInfo,0,sizeof(JniInfo) );
	}
};

//������Ϣ����
struct MsgHeartAliveReq : public PacketBase
{
	unsigned int myindex; //��ʾ�ͻ��˺�gtֱ�ӵ������Ƿ��Ѿ��Ͽ���������ֱ�ӵ�
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

//֪ͨ�ͻ��ˣ��㱻��������[client]
struct MsgNoticeClientKickYouNtf : public PacketBase
{
	unsigned int paramA;   //û��ʹ��
	unsigned int paramB;   //û��ʹ��

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

//[GmAppLogToolView] ������log
struct GmLogToolOrderAppLogReq : public PacketBase
{
	char			account[60];
	char			userDigital[20];  //����ID
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