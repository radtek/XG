#ifndef sgs_server_definblockstruce_h__
#define sgs_server_definblockstruce_h__
//#include "../../../gs_head.h"
//#include "../../../../publib/Common/pool_smallblock.h"
//using namespace sgs_comment;

//#include <boost/preprocessor.hpp>

/*
struct TInGsLitBlock_0
{
char block[2<<0];
};
*/

//////////////////////////////////////////////////////////////////////////
/*
#define LIMITS (0, 10)
#define SAMPLE(n) \
struct  TInGsLitBlock_##n { char block[2<<n]; };\
typedef xpool_smallblock<sizeof(TInGsLitBlock_##n),100000,100> PoolLitBlock_##n;

#define  BOOST_PP_LOCAL_LIMITS   LIMITS
#define  BOOST_PP_LOCAL_MACRO(n) SAMPLE(n)
#include BOOST_PP_LOCAL_ITERATE()
*/

#endif // sgs_server_definblockstruce_h__