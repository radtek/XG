#ifndef Xbuildconfig_tinyxml_h__
#define Xbuildconfig_tinyxml_h__

//#include "../../XDepend/tinyxml-2.5.3/tinyxml.h"
//#include "../../XDepend/tinyxml262/tinyxml.h"
#include "./tinxml/tinyxml.h"

#define LIBNAME "tinyxml"

#ifdef _DEBUG
	#define LIBLINKKIND "mtd"
#else
	#define LIBLINKKIND "mt"
#endif

#ifdef _STLPORT_VERSION
	#define LIB_STLPORTZZ "stlp"
#else
	#define LIB_STLPORTZZ "wstl"
#endif // STLPORT


#define LIB_PEYONENET_LINK_NAME    LIBNAME "_" LIBLINKKIND "_"  LIB_STLPORTZZ

#if defined _MSC_VER
	//#pragma comment(lib, LIB_PEYONENET_LINK_NAME)
#elif defined __GNUC__
	//#pragma message("gun not set linker lib")
#endif

#undef LIBNAME
#undef LIB_STLPORTZZ
#undef LIBLINKKIND
#undef LIBCCKIND
#undef LIB_PEYONENET_LINK_NAME

#endif //Xbuildconfig_tinyxml_h__
