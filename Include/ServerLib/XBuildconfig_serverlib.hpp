
#include "./MySQLDB.h"
#include "../mysql/errmsg.h"
#include "./MSSQLDB.h"

#include "./Thread.h"

using namespace snda;

//for other use
//ifdef USE_PEYONENET_DLL use dll,or use static
#ifndef PEONYXML_DLL_CONFIG
#define PEONYXML_DLL_CONFIG

    #define LIBNAME "ServerLib"

    #ifdef USE_PEYONENET_DLL
        #define LIBLINKKIND "dynamic"
    #else
        #define LIBLINKKIND "static"
    #endif // USE_PEYONENET_DLL

    #ifdef _DEBUG
        //#define LIBKIND "debug"

        #if defined(_MT) && defined(_DLL)
        //use MDD
            //#pragma message("exe--debug--MDD")
            #define LIBCCKIND "mdd"
        #else
        //use MTD 
            //#pragma message("exe--debug--MTD")
            #define LIBCCKIND "mtd"
        #endif

    #else
        //#define LIBKIND "release"

        #if defined(_MT) && defined(_DLL)
        //use MDD
            #define LIBCCKIND "md"
        #else
        //use MTD
            #define LIBCCKIND "mt"
        #endif

    #endif // _DEBUG

	#ifdef STLPORT
		#define LIB_STLPORTZZ "stlport"
	#else
		#define LIB_STLPORTZZ "wstl"
	#endif // STLPORT

    #define LIB_PEYONENET_LINK_NAME  LIBNAME "_" LIBLINKKIND "_" LIBCCKIND "_" LIB_STLPORTZZ ".lib"
    
    #if defined _MSC_VER	
        #pragma comment(lib, LIB_PEYONENET_LINK_NAME)
    #elif defined __GNUC__
        //#	import <lzo2_static-rtl_release.lib>
        #error "Unknown compiler, only support for msvc and gcc."
    #else
        #error "Unknown compiler, only support for msvc and gcc."
    #endif

#undef LIBNAME
#undef LIBLINKKIND
#undef LIBCCKIND

#endif //PEONYXML_DLL_CONFIG
