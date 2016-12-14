#ifndef XDirtyMark_h__
#define XDirtyMark_h__
#include "./SaveFun.h"

class XObjDirty
{
protected:
	XObjDirty(){ m_MDirty=0; m_DirtyTime=0; }
	virtual ~XObjDirty(){}

public:
	virtual void	DirtyX( unsigned int iDirty,bool isPrior );
	void			ClearAllDirty();
	void			ClearDirty( unsigned int iDirty );
	bool			IsDirtyX( unsigned int iDirty ){ return (m_MDirty&iDirty)>0; }
	bool			IsDirtyX(){ return m_MDirty>0; }
	unsigned int    GetDirty(){ return m_MDirty; }
	unsigned int    GetDirtyTime(){ return m_DirtyTime; }

protected:
	unsigned int    m_MDirty;
private:
	unsigned int    m_DirtyTime;
};

class XObjMark
{
protected:
	XObjMark(){ m_XOMark=0; }
	virtual ~XObjMark(){}

public:
	virtual void	SetXMark( unsigned int xmark ){ m_XOMark |= xmark; }
	virtual void	ClearXMark(unsigned int xmark){ m_XOMark &= (~xmark); }
	bool			IsXMark( unsigned int xmark ){ return (m_XOMark&xmark)>0; }
	unsigned int    GetXMarkV(){ return m_XOMark; }

protected:
	unsigned int    m_XOMark;
};
class XScMark
{

public:
	static void	SetXMark(   unsigned int &XOMark,unsigned int xmark ){ XOMark |= xmark; }
	static void	ClearXMark( unsigned int &XOMark,unsigned int xmark){ XOMark &= (~xmark); }
	static bool	IsXMark(    unsigned int  XOMark,unsigned int xmark ){ return (XOMark&xmark)>0; }
};

#endif // XDirtyMark_h__
