//for other use
//ifdef USE_PEYONENET_DLL use dll,or use static
#ifndef Xbuildconfig_frame_h__
#define Xbuildconfig_frame_h__

    #define LIBNAME "xframe"

    #ifdef _DEBUG
		#ifdef STLPORT
			#define LIB_STLPORTZZ "stlpd.lib"
		#else
			#define LIB_STLPORTZZ "wstld_64.lib"
		#endif // STLPORT
    #else
		#ifdef STLPORT
			#define LIB_STLPORTZZ "stlpr.lib"
		#else
			#define LIB_STLPORTZZ "wstlr_64.lib"
		#endif // STLPORT
    #endif // _DEBUG


	#define LIB_XFRAME_LINK_NAME    LIBNAME "_" LIB_STLPORTZZ
    
    #if defined _MSC_VER
        #pragma comment(lib, LIB_XFRAME_LINK_NAME)
    #elif defined __GNUC__
		//#pragma message("gun not set linker lib")
    #endif

#undef LIBNAME
#undef LIB_STLPORTZZ
#undef LIB_XFRAME_LINK_NAME

#endif //Xbuildconfig_frame_h__
