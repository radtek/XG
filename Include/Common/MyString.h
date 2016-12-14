
#if !defined(AFX_MYSTRING_H__04538BAC_0EF7_46EB_AAF7_9201677B5D10__INCLUDED_)
#define AFX_MYSTRING_H__04538BAC_0EF7_46EB_AAF7_9201677B5D10__INCLUDED_

#include "./SaveFun.h"

class CLogicaPub 
{
public:
	static unsigned int  nickname_buffer_to_vector( const char *buffer,unsigned int buffersize,VEC_STR &vstr );
	static unsigned int  nickname_vector_to_buffer( VEC_STR &vstr,char *buffer,unsigned int buffersize);
};

#endif // !defined(AFX_MYSTRING_H__04538BAC_0EF7_46EB_AAF7_9201677B5D10__INCLUDED_)
