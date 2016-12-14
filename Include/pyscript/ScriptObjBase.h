#ifndef ScriptObjBase_h__a
#define ScriptObjBase_h__a
#include "../Common/ZJsonPub.h"

#ifdef XGAME_ZHT_PYTHON
#include <boost/filesystem.hpp>
#include <boost/python.hpp>
#include <boost/function.hpp>


//script interface wrap--------------------------------------------------------------------
#include <boost/python.hpp>

struct PyObjectBase
{
protected:
	PyObjectBase();
public:
	virtual ~PyObjectBase();
};

//外部python对象的基类
class PyZhtOCpp : public PyObjectBase, public boost::python::wrapper<PyZhtOCpp>
{
public:
	PyZhtOCpp();
	virtual ~PyZhtOCpp();

public:
	string        GetMyDoc(){ return "fasfd"; }
	
	//C++调用这个类的Python函数
	string        CppCallPyFun( string funName,zint64 iOne,string strOne,string strTwo );
};
typedef map<string,PyZhtOCpp*>MapPyObj;
extern void export_PyZhtOCpp_py();
typedef map<string,boost::python::object> MapPyFun;

#endif // XGAME_ZHT_PYTHON
#endif // ScriptObjBase_h__
