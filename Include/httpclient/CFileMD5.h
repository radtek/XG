#ifndef CFileMD5_h__
#define CFileMD5_h__

#include "../../Include/Common/TaskQueue.h"
#include "../../Include/Common//md5.h"
class CFileMD5 :public TaskBase
{
public:
	enum RetCode{ 
		eErrNoDef = 0,
		eErrSuccess, 
		eErrRetBufNotOverflow,
		eErrFileOpen,
		eErrCurlInit,
		eErrCurlPerform
	};

	typedef boost::function<void (CFileMD5 *pItem)> Callback;


public:
	CFileMD5(void);
	virtual ~CFileMD5(void);

	virtual void run(void);
	virtual void callback(void);

	bool setFileName(const char * filename);
	
	const char *getMD5(void)const;

	void setCbFun(const Callback &cbFun); 

private:
	char				mFileName[256];
	char				mFileMD5[60];
	int					mRepCode;
	Callback			mCbFun;
};
#endif // CFileMD5_h__
