#ifndef sgs_server_stdhead_h__
#define sgs_server_stdhead_h__

//标准库文件
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>

#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <queue>
#include <algorithm>
#include <functional>
#include <iostream>
#include <istream>
#include <ostream>
#include <sstream>
using namespace std;

//#define _STLPORT_VERSION 0x521
#ifdef STLPORT
	#define SGS_USER_STLPORT 1
	#include "./hash_stlport.hpp"
#else
	#include "./hash_container.hpp"
#endif // _STLPORT_VERSION

#endif // sgs_server_stdhead_h__