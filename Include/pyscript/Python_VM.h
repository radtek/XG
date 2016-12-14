#ifndef PYTHON_VM_H_A
#define PYTHON_VM_H_A
#include "./ScriptObjBase.h"
#ifdef XGAME_ZHT_PYTHON

//script interface wrap--------------------------------------------------------------------
const   string pyscriptinit_init = "init.py";
typedef boost::function< void() > HandlePyInitApp;
typedef boost::function< void(string, boost::python::object&)> HandlePyInitFinish;

typedef list<HandlePyInitApp>PyListAppScript;
typedef list<string>LIST_INITSCRIPT;
struct TInitData
{ 
	LIST_INITSCRIPT     scriptlist;
	HandlePyInitFinish  cbLoadThis;		//����һ���ļ��ص�һ��
	HandlePyInitFinish  cbLoadFinish;	//���е��ļ�������ɺ�ص�
};

class python_vm
{
private:
	typedef boost::function<void (boost::python::object&)> add_py_object_type;
	typedef boost::function<void (std::string, boost::python::object&)> add_py_class_type;
	//typedef std::vector<boost::python::object> py_object_vector_container;

	typedef SgsStl::hash_map<string,boost::python::object>MapPyObjectFactory;
	//��������ʵ������װ�����ͷţ������ⲿ�ſ���ʹ��
	typedef map<PyObjectBase*,boost::python::object>MapPyObjContainer;

	struct TLoadPyFileCon
	{
		add_py_class_type	func;
		bool				isCheckMap;
	};

private:
	python_vm(void);
	~python_vm(void);

public:
	static python_vm *  Instance();

	void                add_initappfun( HandlePyInitApp appinit ); //��������
	int 				Init(string pyRoot,string strPath,TInitData &initdata );
	void				UnInit();

	/************************************************************************
	//���ؽű� ָ��init.py�ļ� �ѡ��������ࡱ��func���� 
	************************************************************************/
private:
	//����һ����
	int					load_script_pack(string strpath,TInitData &initdata,bool isCheckMap);
	//����һ��ģ��
	int					imp_load_onefile( string file_name,string strwhilepath);

	bool                ReloadScript( string strFile );

private:
	bool				IsHaveName( string strPyName );

public:
	//����py�����ʵ������
	template< typename T> T *CreateOneObj( string strObj )
	{
		boost::python::object objfact;
		if( !find_objfactory( strObj,objfact ) ){
			APPLOG_ERROR("[LogicErr,no this pyclass] strObj="<<strObj<<FUN_FILE_LINE );
			return 0;
		}

		try
		{
			boost::python::object Obj = objfact();
			T* ai_ptr                 = boost::python::extract<T*>(Obj);
			PyObjectBase* pBaseObj    = dynamic_cast<PyObjectBase*>(ai_ptr);
			if( pBaseObj ){
				m_map_PyObjInstance[pBaseObj] = Obj;
				return ai_ptr;
			}else{
				APPLOG_ERROR("[LogicErr,pyclass type error] strObj="<<strObj<<FUN_FILE_LINE );
				return 0;
			}
		}
		catch(boost::python::error_already_set const&){
			APPLOG_ERROR("[Py Exception....] strObj="<<strObj<<";objfact="<<typeid(objfact).name()<<FUN_FILE_LINE );
			PyErr_Print();
			return 0;
		}catch(...)
		{
			APPLOG_ERROR("[Py Exception...bbb.] strObj="<<strObj<<FUN_FILE_LINE );
			return 0;
		}
	}
	void DeleteOneObj(PyObjectBase* pBaseObj)
	{
		MapPyObjContainer::iterator it = m_map_PyObjInstance.find( pBaseObj );
		if( it==m_map_PyObjInstance.end() ){
			APPLOG_ERROR("[Logic error...] pBaseObj="<<typeid(*pBaseObj).name()<<FUN_FILE_LINE );
			return;
		}
		m_map_PyObjInstance.erase( pBaseObj );
	}

private:
	bool find_objfactory( string strObj,boost::python::object &obj );

private:
	string                          m_strPath;
	MapPyObjectFactory              m_map_npc_factory;		//<�ű�������,  �ű��๤��>
	MapPyObjContainer				m_map_PyObjInstance;	//�ڴ��д����Ķ���ʵ��
	boost::python::object           m_global;

};

#endif // PYTHON_VM_H
#endif // XGAME_ZHT_PYTHON
