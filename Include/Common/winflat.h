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
#ifdef OS_WINDOWS_ZHT //// windows 平台的宏定义 ///////////////////////////////////////
	#define WIN32_LEAN_AND_MEAN		// 从 Windows 头中排除极少使用的资料

	// Windows 头文件:
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

//删除一个内存指针
#define  DELETE_DATA(POINT) while(POINT){ delete POINT;POINT =0 ; break;}

/********************************************************************
created:	2010-7-8   10:06 author: Zhanghongtao
purpose:	定义模块的自动测试宏定义
*********************************************************************/
//#define SGS_AUTO_TEST 1

//如果被定义，只进行逻辑错误检查，不允许修改程序的流程。
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