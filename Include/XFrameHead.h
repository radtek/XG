/********************************************************************
	created:	24:6:2010   16:55
	author:		Zhanghongtao
	purpose:	定义服务器共用的
*********************************************************************/
#ifndef _DEFINE_GAME_GLOBAL_HEAD_XFRAMEHEAD_
#define _DEFINE_GAME_GLOBAL_HEAD_XFRAMEHEAD_

#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/filesystem.hpp>
#include <boost/noncopyable.hpp>
#include <boost/pool/object_pool.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#ifdef WIN32
	#pragma warning( push )
	#pragma warning( disable : 4819 )
	#include <boost/format.hpp>
	#pragma warning( pop ) 
#else
	#include <boost/format.hpp>
#endif
typedef boost::recursive_mutex::scoped_lock  Boost_Scoped_Lock;


//comment
#include "../Include/Common/winflat.h"
#include "../Include/Common/BitSet.h"
#include "../Include/Common/SaveFun.h"
#include "../Include/Common/ZJsonPub.h"
#include "../Include/Common/build_time.h"
#include "../Include/Common/WndConsole.h"
#include "../Include/Common/SaveControl.h"
#include "../Include/Common/svn_version.h"
#include "../Include/Common/exceptionreport.h"
#include "../Include/Common/timeouts_queue.hpp"
#include "../Include/Common/get_app_stackinfo.h"
#include "../Include/Common/get_app_stackinfo_win.h"

//allocmen
#include "./allocmem/alloc_basemg.h"

//base64
#include "./base64/base64.h"

//config
#include "./config/baseConfig.h"
#include "./config/CSRedisCfg.h"

//control
#include "./control/msg_channel_define.h"
#include "./control/NoGtHeartCheck.h"
#include "./config/baseConfig.h"
#include "./config/CSRedisCfg.h"

//http
#include "./httpclient/CHttpMg.h"

//httpSs
#include "./HttpSs/HttpRep.h"
#include "./HttpSs/HttpSs.h"

//json
#include "./json/json.h"
#include "./json/json_tool.h"

//log
#include "./netlog/impnetlog.h"

//mongdb
#include "./mongdb/mongdbface.h"

//MnHiredThMg
#include "./mongdb_hiredis_mg/HiredisThMg.h"
#include "./mongdb_hiredis_mg/MongdbThMg.h"

//mysql
#include "./mysql/mysql.h"

//mysqlbase
#include "./mysqlbase/MySQLDBSys.h"

//redis
#include "./hiredis/RedisFace.h"

//sqlite3
//#include "./sqlite3/sqlite3ext.h"
#include "./sqlite3/sqlite3.h"

//timebuffer
#include "./TimeBufMg/ImpTempBufferMg.h"
#include "./TimeBufMg/TempBufferMg.h"

//timer
#include "./timer/TimerMg.h"

//tinxml
#include "./tinxml/tinyxml.h"

//xframprotocol
#include "./xframe_protocol/anti_addiction_protocol.h"

//zip
#include "./zip/zlib.h"

//control
#include "./control/SysStatistics.h"
#include "./control/NoGtHeartCheck.h"
#include "./control/msg_channel_define.h"
#include "./control/SgsServerControlInfo.h"

#include "./ComboxEvent/ComboxEvtMg.h"
#include "./DistrFramePub/DFunInterMg.h"

//Python
#ifdef XGAME_ZHT_PYTHON
#include "./pyscript/Python_VM.h"
#include "./pyscript/zhtPyIFaceMg.h"
#endif // XGAME_ZHT_PYTHON

#endif