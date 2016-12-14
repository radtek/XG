/********************************************************************
	created:	1:3:2011   10:06
	author:		zhanghongtao
	
	purpose:	提供信道的基本属性，定义
*********************************************************************/

#ifndef msg_channel_define_h__
#define msg_channel_define_h__
#include "../../Include/Common/winflat.h"

//定义信道的类别，最多255个
enum EmMsgChanelType
{
	msgchaneltype_all_user      = 1,              //全国所有人呢,属于系统信道
	msgchaneltype_fs            = 3,              //fs建立的信道
	msgchaneltype_cs            = 4,              //cs建立的信道
	msgchaneltype_gs            = 5,              //gs建立的信道
	msgchaneltype_dbs           = 6,              //dbs建立的信道
};

#pragma pack(push)
#pragma pack(1)
//分离式的信道id
struct TChanelIDNickName
{
public:
	unsigned char   chanelid_one;         //可以自己定义
	unsigned char   chanelid_two;         //可以自己定义
	unsigned char   chanelid_three;       //可以自己定义
	unsigned char   chaneltype;           //EmMsgChanelType
};
#pragma pack(pop)


struct TMsgChanelItem
{
public:
	static const unsigned int sc_chn_max_hashkey = 20;
	//信道id的定义
	union UChanelID
	{
		unsigned int       id;
		TChanelIDNickName  idnk;  //分离是ID
	};

	UChanelID      channelid;
	char           desc[32];           //信道说明

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
	unsigned int   uhashkey;           //hash用户的key

};


class CMsgChannelPub
{
public:
	static TChanelIDNickName channelid_to_channelid(unsigned int chnanelid);
	static unsigned int		 cal_channelid( unsigned char type,unsigned char one,unsigned char two,unsigned char three);
};

//全区频道
static const unsigned int sc_msgchannelid_alluser = CMsgChannelPub::cal_channelid(msgchaneltype_all_user,0,0,0);

#endif // msg_channel_define_h__