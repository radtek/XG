#ifndef base64_h__
#define base64_h__
#include "../../Include/Common/xframe_types.h"
#include "../../Include/Common/winflat.h"

class Base64
{
public:
	static size_t Base64_Decode(char *pDest, const char *pSrc, size_t srclen);
	static size_t Base64_Encode(char *pDest, const char *pSrc, size_t srclen);
};

#endif // base64_h__

