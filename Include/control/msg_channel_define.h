/********************************************************************
	created:	1:3:2011   10:06
	author:		zhanghongtao
	
	purpose:	�ṩ�ŵ��Ļ������ԣ�����
*********************************************************************/

#ifndef msg_channel_define_h__
#define msg_channel_define_h__
#include "../../Include/Common/winflat.h"

//�����ŵ���������255��
enum EmMsgChanelType
{
	msgchaneltype_all_user      = 1,              //ȫ����������,����ϵͳ�ŵ�
	msgchaneltype_fs            = 3,              //fs�������ŵ�
	msgchaneltype_cs            = 4,              //cs�������ŵ�
	msgchaneltype_gs            = 5,              //gs�������ŵ�
	msgchaneltype_dbs           = 6,              //dbs�������ŵ�
};

#pragma pack(push)
#pragma pack(1)
//����ʽ���ŵ�id
struct TChanelIDNickName
{
public:
	unsigned char   chanelid_one;         //�����Լ�����
	unsigned char   chanelid_two;         //�����Լ�����
	unsigned char   chanelid_three;       //�����Լ�����
	unsigned char   chaneltype;           //EmMsgChanelType
};
#pragma pack(pop)


struct TMsgChanelItem
{
public:
	static const unsigned int sc_chn_max_hashkey = 20;
	//�ŵ�id�Ķ���
	union UChanelID
	{
		unsigned int       id;
		TChanelIDNickName  idnk;  //������ID
	};

	UChanelID      channelid;
	char           desc[32];           //�ŵ�˵��

public:
	TMsgChanelItem()                { channelid.id = 0;  memset( desc,0,sizeof(desc)); uhashkey=10; }
	TMsgChanelItem(unsigned int id) { channelid.id = id; memset( desc,0,sizeof(desc)); uhashkey=10; }

public:
	unsigned int get_haskkey() const{ return uhashkey; }
	void         set_haskkey(unsigned int hashkey )
	{ 
		uhashkey=hashkey;
		if( uhashkey>sc_chn_max_hashkey )
			uhashkey=sc_chn_max_hashkey;
		if( uhashkey<1 )
			uhashkey=uhashkey;
	}

private:
	unsigned int   uhashkey;           //hash�û���key

};


class CMsgChannelPub
{
public:
	static TChanelIDNickName channelid_to_channelid(unsigned int chnanelid);
	static unsigned int		 cal_channelid( unsigned char type,unsigned char one,unsigned char two,unsigned char three);
};

//ȫ��Ƶ��
static const unsigned int sc_msgchannelid_alluser = CMsgChannelPub::cal_channelid(msgchaneltype_all_user,0,0,0);

#endif // msg_channel_define_h__