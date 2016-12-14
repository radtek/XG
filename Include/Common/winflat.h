/********************************************************************
	created:	18:6:2010   12:22
	filename: 	F:\SGS_3\Common\winflat.h
	author:		Zhanghongtao
	
	purpose:	
*********************************************************************/
#ifndef _DEFINE_PUBLIC_WINPLAT_HEAD_
#define _DEFINE_PUBLIC_WINPLAT_HEAD_

#ifndef WIN32
	#define OS_LINUX_ZHT
#else
	#define OS_WINDOWS_ZHT
#endif

#include "./stdhead.h"
#include <memory.h>
#include "./xframe_types.h"
#ifdef OS_WINDOWS_ZHT //// windows ƽ̨�ĺ궨�� ///////////////////////////////////////
	#define WIN32_LEAN_AND_MEAN		// �� Windows ͷ���ų�����ʹ�õ�����

	// Windows ͷ�ļ�:
	#include <windows.h>
	#include <sys/types.h>
	#include <sys/timeb.h>
	#include <mmsystem.h>


	/////////////////////////////////////////////////////////////////////////////
	#pragma comment(lib,"ws2_32.lib")
	#pragma comment(lib,"Winmm")

	#ifndef _USE_32BIT_TIME_T
		//#define _USE_32BIT_TIME_T
	#endif

	#include <psapi.h>
	#pragma comment(lib,"psapi.lib")

	//include/iconv.h
	//#pragma comment(lib,"iconv.lib" )

#endif

#ifdef OS_LINUX_ZHT
	using namespace __gnu_cxx;
	
	#include <stddef.h>
	#include <time.h>
	#include <stdio.h>
	#include <sys/types.h> 
	#include <sys/timeb.h>

#endif //OS_LINUX_ZHT

//////////////////////////////////////////////////////////////////////////
/// Disable warning
//////////////////////////////////////////////////////////////////////////
#ifdef WIN32
	#pragma warning( disable : 4996 )
	#pragma warning( disable : 4267 )
	#pragma warning( disable : 4251 )
#endif

//ɾ��һ���ڴ�ָ��
#define  DELETE_DATA(POINT) while(POINT){ delete POINT;POINT =0 ; break;}

/********************************************************************
created:	2010-7-8   10:06 author: Zhanghongtao
purpose:	����ģ����Զ����Ժ궨��
*********************************************************************/
//#define SGS_AUTO_TEST 1

//��������壬ֻ�����߼������飬�������޸ĳ�������̡�
#define SGS_LOGIC_RIGHT_CHECK	1
#define XGAME_ZHT_PYTHON		1

#ifdef _WIN32
#ifdef _DEBUG
#pragma comment(lib, "hiredis_d64.lib")
#else
#pragma comment(lib, "hiredis_r64.lib")
#endif
#endif //_WIN32

#endif