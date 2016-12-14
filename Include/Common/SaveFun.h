/********************************************************************
	created:	31:5:2010   13:15
	filename: 	f:\SGS_2\Server\Common\SaveFun.h
	author:		Zhanghongtao
	
	purpose:	
*********************************************************************/
#ifndef _SAVE_FUN_
#define _SAVE_FUN_
#include "./xframe_types.h"
#include "./winflat.h"
#include <boost/bind.hpp>
#include <boost/timer.hpp>
#include <boost/random.hpp>
#include <boost/date_time.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/assign/std/list.hpp>
#include <boost/assign/std/vector.hpp>
#include <boost/numeric/conversion/converter.hpp>
using namespace boost::assign;

#include "../../Include/log/interfacelog.h"
#include "../../Include/netlog/interfacenetlog.h"

#ifdef WIN32
	#pragma warning( push )
	#pragma warning( disable : 4819 )
	#pragma warning( disable : 4244 )
	#include <boost/format.hpp>
	#include <boost/thread.hpp>
	#pragma warning( pop )
	#pragma warning( disable : 4724 )
	
	#include <WinSock2.h>
#else
	#include <boost/format.hpp>
	#include <boost/thread.hpp>
#endif


//#ifndef _BPI_ARMV7
#define STATIC_LIBMONGOCLIENT
#include "../../../XDepend/mongodbcxx104/include/mongo/client/dbclient.h"
using namespace mongo;
// #endif

const unsigned int sc_oneday_timelen = 3600*24;
struct TBufferItem
{
	int			datalen;
	char *      buf;

	TBufferItem()
	{
		buf     = 0;
		datalen = 0;
		buflen  = 0;
	}
	void ClearData()
	{
		datalen = 0;
		memset(buf,0,buflen);
	}
	void SetMaxLen( int v){ buflen=v; }
	int  MaxLen(){ return buflen; }
	bool CopyFrom( TBufferItem *pOther )
	{
		if( pOther->buf == this->buf )
			return true;
		if( pOther->datalen+2>MaxLen())
			return false;
		memset(buf,0,buflen);
		memcpy( buf,pOther->buf,pOther->datalen);
		datalen = pOther->datalen;
		return true;
	}
	bool CopyFrom( const char *pData, int iDataLen )
	{
		if( pData == this->buf )
			return true;
		if( iDataLen+2>MaxLen())
			return false;
		memset(buf,0,buflen);
		memcpy( buf,pData,iDataLen );
		datalen = iDataLen;
		return true;
	}
private:
	int  buflen;
};

extern TBufferItem g_bufaa;
extern TBufferItem g_bufbb;
extern TBufferItem g_bufcc;
extern unsigned    g_threadsleeptm; //�߳����ߵ�ʱ��

//////////////////////////////////////////////////////////////////////////
//��������vect��һЩ������
typedef list<unsigned int>LIST_UINT;
typedef list<zint64>LIST_ZINT;
typedef list< string >   LIST_STRING;
typedef vector<unsigned int>VECTOR_UINT;
typedef vector<int>VECTOR_INT;
typedef vector<unsigned char>VECTOR_UCHAR;
typedef vector< string > VEC_STR;
typedef set<unsigned int> SET_UINT;
typedef map<unsigned int,unsigned int>MAP_UINT;
typedef map<unsigned int,VECTOR_UINT>MAP_VECTORUINT;
typedef map<string,unsigned int>MAP_STRTOUINT;
typedef map<string,unsigned int>MAP_SSSTR;
typedef map<string,string> MAP_XMLKEY;

bool stl_opfun_big( unsigned int des, unsigned int sour );
bool stl_opfun_equal( unsigned int des,unsigned int sour );
bool stl_opfun_little( unsigned int des, unsigned int sour );
bool stl_opfun_del( VECTOR_UINT &vec,unsigned int del_v );

typedef boost::function<void ()> hander_void_xcall;
typedef boost::function<void (int iErrPos)> hander_void_xcallB;
typedef boost::function<string( zint64 iOne,string strOne,string strTwo )>HandCFunAForPy;

class CSgsPubFun
{
public: //ʱ�����
	static void			SetLowCpuModel(int iGrade=0/*1---10*/);
	/********************************************************************
		created:	1:7:2010   15:24
		return:     ������������ʾʱ��. ��λ�� ��.	
	*********************************************************************/
	static unsigned int GetNowTime();
	static unsigned int GetNowDateDay();					 //�����Ǽ��� 0-31;
	static unsigned int GetNowDateWDay();					 //�������ڼ�(0==�����죬1==����һ���ȵ�)
	static unsigned int GetDateWDay(unsigned int nowtime );  //�������ڼ�(0==�����죬1==����һ���ȵ�) 
    static unsigned int GetTodayZeroHourZeroMinute();        //�����0��0����
	static unsigned int GetTomorrowZeroHourZeroMinute();     //�����0��0����
	static unsigned int InLocalGetNowTime();
	static unsigned int GetThreadID( boost::thread::id aid );
	/********************************************************************
	created:	GetServerRelative
	return:     �õ�������������ʱ��. ��λ�� ��/100. ����0.01��,�û�����
	            һЩ��Ϣ����ʱ�侫��,
				������������о�����һ��
	*********************************************************************/
	static unsigned int GetServerRelative(bool isupdate=false);                  

	/********************************************************************
		created:	2010-10-18   10:47 author: Zhanghongtao
		purpose:	"%d-%d-%d %d:%d:%d"
					"��-��-�� ʱ:��:��"
    *********************************************************************/
    static string       GetNowTimeToStr();
	static string       GetNowTimeToStrYMD();           //������
	static string       GetNowTimeToStrYMD_B();         //������2010-10-18
	static unsigned int GetCurTime_YMD();               //���ص�ǰ�����յ�����160126; 6λ����,16��1��26��
	static string       GetNowTimeToStrTwo();
	static string		GetTimeToStrYMD( unsigned iTime );   //������ 20160321
	static string       GetTimeToStr( unsigned iTime );   //������ʱ���� 2010-10-18 12:00:00

	static int          GetCurWeekCount( int years );
	//years=0 ���꣬=1 ȥ��, =2 ǰ��,
	static void         GetWeekBeginEndTime( int years,int week,int &begintime,int &endtime );
	static void         GetCurWeekBeginEndTime( unsigned &begintime,unsigned &endtime );

	static void         GetMonthBeginEndTime( int years,int iMonth,int &begintime,int &endtime );
	static void         GetCurMonthBeginEndTime( int &begintime,int &endtime );

	static int          GetCurDay();	//��ǰ����������µĵڼ���
	static int          GetCurWeek( unsigned int uCurTime );	//
	static int          GetCurMonth();	//
	static int          GetCurYear();	//2016
	static int          GetCurMonthPreMonth(int iPreCt,int &begintime,int &endtime);//��ǰ�µ�ǰ1���£�2����

	/********************************************************************
	created:	2010-10-18   10:47 author: Zhanghongtao
	��һ��ʱ��ת��Ϊ��ǰ��ʱ�䣬������󷵻�0; 
	eg: 122520,12��25����20��-->ת��Ϊ�����12��25����20��;
	������ʽ:  120528; 12��5��28��
	*********************************************************************/
	static unsigned int h_m_s_str_to_curtime( string strv );


	/********************************************************************
	created:	2010-10-18   10:47 author: Zhanghongtao
	������ʽ:  2011-3-18 3:58:02
	������ʽ:  2015/10/14 10:41:52
	����ĸ�ʽ������
	*********************************************************************/
	static unsigned int ymd_hms_str_to_curtime( string strv );

	/********************************************************************
	created:	2010-10-18   10:47 author: Zhanghongtao
	��һ��ʱ��ת��Ϊ��ǰ��ʱ�䣬������󷵻�0; 
	������ʽ: 20101209T122520,  2010��12��9��12��25����20��-
	*********************************************************************/
	static unsigned int ymd_h_m_s_str_to_curtime( string strv );

	/********************************************************************
	created:	2010-10-18   10:47 author: Zhanghongtao
	���ʱ���ʽ�Ƿ���ȷ,�����ȷ���غϷ��ַ�����������󷵻ؿ�
	������ʽ: 20101209 ����
	          122520,  ʱ��
	*********************************************************************/
	static string format_right_ymd( string strv ); //����
	static string format_right_hms( string strv ); //ʱ��
	static string format_right_ymdhms( string strv ); //ʱ��

	/********************************************************************
	created:	2010-10-18   10:47 author: Zhanghongtao
	purpose:	���¼���ʽΪ�����յ�ʱ���ʽתΪ��׼��ʱ���ʽ 19780903 -> 1978-09-03
	             ������ɫ��ʱ��ʹ��
	*********************************************************************/
    //"20110505" to 20110505
	static unsigned int FormatDateStrToUint( const char *pstrsource );
    //Ŀ���ʽ 2011-3-12 1:23:21
	static string       FormatDateUintToStr( unsigned int datev );
	//160312,012321
	static string       FormatTmToYMDHMS( unsigned int datev );
	static string       FormatTmToYMD( unsigned int datev );

    /**
    * created: 2011-04-06 by ddd
    * pstrdata: "20110406"��ָ��
    * pstrtime: "073006"��ָ��
    * return: ʱ��. ��λ��. ���󷵻�0.
    */
    static unsigned int FormatDateTimeToUint( const char *pstrdata, const char *pstrtime);


	/********************************************************************
	created:	2010-6-13   11:25 author: Zhanghongtao
	return:     �����������������ڵ�ʱ��: ��λ΢��,   1�� = 1000 000 ΢��
	purpose:    һ���������㺯��������ʱ��
	*********************************************************************/
	static ui64     GetTime_Precision();

	static i64      strToInt64No(const char *psource );
	static i64      strToInt64(const char *psource , bool &iserro);
	static string   Int64ToStr( i64 vvv );

	static ui64     strToUInt64No(const char *psource );
	static ui64     strToUInt64(const char *psource , bool &iserro);
	static string   UInt64ToStr( ui64 vvv );
	//test
	static void     testtime();
	static unsigned int	get_hash_index( string v );

	static int      rc4_encrypt( unsigned char *pData,unsigned int datalen );
	static string   encrypt_small( const char *psour,unsigned int sourlen );
	static bool     decryption_small( string &sourstr, char *pdest,unsigned int destlen );
	static tm       usecond_to_tm( unsigned int total_second ); //��һ��19700101�����ڵ���ת��Ϊ

	static string   md5_base64( string &sourStr,bool isLow ); //����sourStr��md5��Ȼ��base64,isLow�����д������Сд
	static string   md5_Hex( string &sourStr,bool isLow );    //����sourStr��md5��Ȼ��base64,isLow�����д������Сд
	static string   md5_HexB( const char *pSour,unsigned iSourLen,bool isLow ); //����sourStr��md5��Ȼ��base64,isLow�����д������Сд
	static string   ByteToHexChar( unsigned char byte_arr[], int arr_len );

	static string   PingAppHttp(unsigned uConId, IHttpReq *pHttpReq);

	static string   GetCurThreadID();

private://ʱ��ת������
	static time_t   tm_to_timet( tm &mytm );
	static time_t   usecond_to_timet( unsigned int total_second ); //��һ��19700101�����ڵ���ת��Ϊ
	static time_t   boostptime_to_timet( boost::posix_time::ptime &myptime );
	
	static tm       timet_to_tm( time_t &mytime );
	static tm       boostptime_to_tm( boost::posix_time::ptime &myptime );
	
	static boost::posix_time::ptime tm_to_boostptime( tm &mytm );
	static boost::posix_time::ptime timet_to_boostptime( time_t &mytime );
	static boost::posix_time::ptime usecond_to_boostptime(  unsigned int total_second );
	static boost::gregorian::date   usecond_to_boostdate( unsigned int total_second );
	static unsigned int             boostptime_to_usecond( boost::posix_time::ptime &myptime );

public://�ַ����й�

	// "1001" --->9
	static bool				BinstrToUint(const string source, unsigned int &ret );
	static unsigned int     strToUintNo(const char *psource);
	static unsigned int     strToUint(  const char *psource,bool &iserror );
	static int              strToIntNo( const char *psource );
	static int              strToInt(   const char *psource,bool &iserror );
	static float			strToFloat(const char* pStr);
	static float			strToFloat(const string& sSource);

	static bool				strcpy(char *pDes,unsigned int umax,const char *pSour,unsigned int icalpos=100);
	static bool				strcpyb(char *pDes, unsigned int umax,string &strSour,unsigned int icalpos=100);
	static bool				strcpy_part(char *pDes,unsigned int umax,const char *pSour,unsigned int icalpos=100);
	static string           trim_leftright_space( string stritem );
	static VEC_STR			SpliteStrToVector(const std::string& sSource, char delim);
	static VEC_STR			SpliteStrToVector( const std::string& sSource, string delim, string null_subst="");
	static void         	SpliteStrToVector(VEC_STR &vStr, const std::string& sSource, string delim, string null_subst="");
	static unsigned short   strToUshort(const char *psource );
	static unsigned short   strToUshort(string source);
	static short            strToShort( string source);
	static bool             strToUshort( string source,unsigned short &retv );


	//�������
	static void             DebutUtf8( const char *pvvv );
	static string           UTF8ToGBK(const char *sour);
	static string           GBKToUTF8(const char *sour);
	static string           GBKToUTF8B( string strSour ){ return GBKToUTF8(strSour.c_str()); }
	static size_t			GBKToUTF8(string strSour,char**des,size_t &desmaxlen);
	static size_t			GBKToUTF8(const char *sour,size_t sourlen,char**des,size_t &desmaxlen);
	static size_t			UTF8ToGBK(const char *sour,size_t sourlen,char**des,size_t &desmaxlen);

	//�Ƿ���ֻ�����ֵ��ַ���
	static bool				is_digit( const char *sour );
	static bool				is_digit( const string sour);

//ϵͳ���
public:
	static string			get_exedir();
	static string			get_exename();
	static string			get_exenameNoServerId();
	static unsigned int		rand_uint(unsigned int min,unsigned int max);
	static int              randint(int a,int b);
	static void				SetExceptionLogFile(string ssmark,class ExceptionReport * pexception);

	//return WorkingSetSize, ��λM
	static unsigned int		get_process_memory_info(unsigned int & PagefileUsage,unsigned int & PagefileUsageK );

	//�ж��ļ��Ƿ����
	static bool             file_isexist(string strfilepath );

	//�õ�pid
	static unsigned int     get_self_pid();

public:
	static bool             me_alive( unsigned int nowtime,unsigned int dowithmsgcount );

public:
	//����ture��ʾroll�ɹ�,��֮��Ȼ��ֻ�ܾ�ȷ��1/1000(0.001f) fChance[0-100.0f]
	static bool				roll_chance_f(float fChance);

public:
	static int              gzcompressfile( string strSourFile,string strDesFile );
	static int              gzcompress(     char *dest, unsigned int  destLen,char *zdata, unsigned int *nzdata);
	static int              gzdecompress(   char *zdata,unsigned int  nzdata, char *data,  unsigned int *ndata);

private:
	static int				zip_compress(   char *dest, unsigned int *destLen,const char *source, unsigned int sourceLen );
	static int				zip_uncompress( char *dest, unsigned int *destLen,const char *source, unsigned int sourceLen );

public:
	static string			base64_Encode( char *dest,unsigned int destLen );

public:
	static bool				xml2map(string strxml,MAP_XMLKEY& map_keys);
	static string			map2xml(MAP_XMLKEY &map_keys);
	static string			getkeyvalue_frommap(string strkey, MAP_XMLKEY &map_keys );
	static void				Debut64Mem(int iG);

	static int              SysAllocMem( TBufferItem &item,unsigned int iNeedLen );
	static int              SysFreeMem(  TBufferItem &item );
	static TBufferItem *    SysAllocMemAll( unsigned int iNeedLen, int calPos=0 );
	static int              SysFreeMemAll(  TBufferItem *pItem );
	static TBufferItem&     GetSysBuf( int iNeedLen );
	static void             FreeSysBuf();
};

class CAutoTest_Rand
{
public:
	CAutoTest_Rand(){}
	~CAutoTest_Rand(){}
public:
	string test_Rand(unsigned int min,unsigned int max,unsigned int maxloop)
	{
		unsigned int uint_len     = test_rand_uint(min,max,maxloop);
 		ostringstream strlog;
 		strlog<<"uint_len="<<uint_len;
 		return strlog.str();
	}
private:
	unsigned int test_rand_uint(unsigned int min,unsigned int max,unsigned int maxloop)
	{
		return 0;
	}
};

/********************************************************************
created:	2010-10-14   11:30 author: Zhanghongtao
purpose:	���㵥λʱ������һЩ����Ĵ���,�������1�����ڴ����ٱ���һ���û�������
*********************************************************************/
class TControlUnitTimeOpCount
{
public:
	TControlUnitTimeOpCount( string desc="control" );
	~TControlUnitTimeOpCount();

	void				run();
	void                add_docount(unsigned int count){ m_do_count+=count; }
	const unsigned int	get_docount() const{ return m_do_count; }
    void				set_unittime(float second_you );
	void                print();

private:
	float         m_unittime;  //���Եĵ�λʱ�䣬��
	unsigned int  m_do_count;  //���Ĵ���
	boost::timer  m_timer;     //��ʱ��
	string        m_strdesc;
};

class CAppDeadlock
{
public:
	CAppDeadlock(string strFunName);
	~CAppDeadlock(void);
private:
	string m_strFunName;
};

class CAutoInitSysMem
{
public:
	CAutoInitSysMem()
	{
		CSgsPubFun::SysAllocMem( g_bufaa,1024*300 );
		CSgsPubFun::SysAllocMem( g_bufbb,1024*300 );
		CSgsPubFun::SysAllocMem( g_bufcc,1024*300 );
	}
};

typedef boost::function<void()> handle_auto_callfun;
class CAutoCallFun
{
public:
	CAutoCallFun(handle_auto_callfun cb, bool *piscall=0):m_cb(cb),m_piscall(piscall){}
	~CAutoCallFun()	{if( (0==m_piscall) || ( m_piscall && (*m_piscall ) ) ){ m_cb();}}
private:
	handle_auto_callfun	m_cb;
	bool             *  m_piscall; //�Ƿ�Ҫ������Ϣ
};

class CZAppTimer
{
public:
	CZAppTimer();
	~CZAppTimer(){}

	float elapsed();
	void  restart();

private:
	unsigned  m_LastTime;
};

#endif

