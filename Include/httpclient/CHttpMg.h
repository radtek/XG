/********************************************************************
	created:	22:4:2016   10:18
	filename: 	F:\Azserver\XFrame\Frame\httpclient\CHttpMg.h
	author:		ZhangHongtao  email: 1265008170@qq.com
	
	purpose:	支持http的一些简单请求，和发送,
*********************************************************************/
#ifndef CHTTPMG_H_
#define CHTTPMG_H_

#include "../Common/SaveFun.h"
#include "../../Include/Common/timeouts_queue.hpp"
#include "../../Include/Common/TaskQueue.h"
#include "./CFileMD5.h"


// Usage: 
//
//   CZtyHttpMg
//   CZtyHttpMg::single().start();
//   CZtyHttpMg::single().runTime();    
//   CZtyHttpMg::single().stop();
//
//
//   HTTP GET :
//	   void onBaiduReturn(THttpItem *item) {
//			if (!item->isOutFile())
//				APPLOG_NORMAL(item->getOutBuf().buf);
//			else 
//				APPLOG_NORMAL(item->getOutFile());
//	   }
//     void TestHttpGet(void) {
//			THttpItem *item = THttpItem::create("www.baidu.com",false);
//			item->setOutBufLen(1024*10);     // item->setOutFile("f:/baidu.htm");
//			item->setCbFun(onBaiduReturn);
//			CZtyHttpMg::single().addReq(item);
//     }
//     void TestSyncHttpGet(void) {
//			THttpItem item;
//			item.syncInit("www.baidu.com"); 
//			item.setOutFile("f:/baidu.htm");	// item.setOutBufLen(1024*10);
//			int err = item.syncRequest();
//     }
//
//
//   HTTP POST :
//	   void onBaiduReturn(THttpItem *item) {
//			APPLOG_NORMAL(item->getOutBuf().buf);
//	   }
//     void TestHttpPost(void) {
//			THttpItem *item = THttpItem::create("www.baidu.com",true);
//			item->setInData("hello=1343&test=2343&good=0",strlen("hello=1343&test=2343&good=0"));
//				------TBufferItem &inBuf = item->setInDataLen(1000);
//				------strcpy(inBuf.buf,""hello=1343&test=2343&good=0");
//			item->setOutBufLen(1024*10); 
//			item->setCbFun(onBaiduReturn);
//			CZtyHttpMg::single().addReq(item);
//     }
//
class THttpItem : public TaskBase
{
public: 
	enum EmIsUpdateDW
	{
		EmIsUpdateDW_NoDef = 0,
		EmIsUpdateDW_Yes,
	};

	enum { InvalidTimeoutSecs = -1 };

	enum RetCode{ 
		eErrNoDef = 0,
		eErrSuccess, 
		eErrRetBufNotOverflow,
		eErrFileOpen,

		eErrCurlInit = 1000000
	};

	typedef boost::function<void (THttpItem *pItem)> Callback;
	typedef boost::function<int (THttpItem *pItem,double dltotal, double dlnow,double ultotal, double ulnow)>  ProgressCallback; 

public:
	THttpItem(void);
	virtual ~THttpItem(void);

	// sync
	bool	syncInit(const char *url,bool post=false, bool isWithHeader=true);
	int		syncRequest(void);

	// async create
	static	THttpItem* create(const char *url,bool post=false, bool isWithHeader=true);

	//
	bool isPost(void) const;
	const char* getUrl(void) const;

	int timeoutSecs(void) const;
	void setTimeoutSecs(int secs);

	TBufferItem&		setInDataLen(int len);
	void				setInData(const char *data,int len);
	const TBufferItem&	getInData(void) const;

	bool	isOutFile(void) const;
	const TBufferItem& getOutBuf(void) const;
	const char* getOutFile(void) const;
	void	setOutBufLen(int len);
	bool	setOutFile(const char *file);

	void	setCbFun(const Callback &cbFun); 
	void	setCbProgress(const ProgressCallback &cbProgress); 

	int		getHeadRepCode(void) const;
	int		getRepCode(void) const;

	void	setThisIsUpdate() { this->SetXMark(EmIsUpdateDW_Yes); }
	bool	IsUpdate() {  return this->IsXMark(EmIsUpdateDW_Yes); }
	void    setPhpMd5( string strvv ){ CSgsPubFun::strcpy(mPhpFileMd5,sizeof(mPhpFileMd5),strvv.c_str()); }
	string  getPhpMd5(){ return string(mPhpFileMd5); }
	bool	setUrl(const char *url,bool post, bool isWithHeader=true); 
private:
	void freeInOutBuf(void);

	virtual void run(void); 
	virtual void callback(void);

	static size_t CbHeaderFun_XCommon_NoDowith( char *buffer,size_t size,size_t nitems,void *outstream );
	static size_t curlWriteCallback(char *buffer,size_t size,size_t nitems,void *outstream);
	static int	  curlProgressCallback(void *inst,double dltotal, double dlnow,double ultotal, double ulnow);

	int					mTimeoutSecs;
	bool				mPost;
	bool				mWithHeader;//是否带Head
	char				mUrl[512];
	TBufferItem			mInBuf;	  
	TBufferItem         mOutBuf; 
	FILE*				mOutFile;
	char				mOutPath[256];
	Callback			mCbFun;
	ProgressCallback    mCbProgress;
	int					mHeadRepCode;
	int					mRepCode;
	char                mPhpFileMd5[40];
}; 

class CZtyHttpMg  
{
protected:
	CZtyHttpMg();
	~CZtyHttpMg();

public:
	static CZtyHttpMg& Single();

	bool start(unsigned threadCount);
	void stop(void);

	void addReq(THttpItem *item);

	void runTime(int cnt=10);
	unsigned itemInQ(void);

private:
	TaskQueue m_taskQ;

};

typedef XThreadSaveQueue<TaskBase*>  httpItemQueue;

class CSingleMTPHttp  
{
	enum EmInUpdate
	{
		EmInUpdate_Nodef = 0,
		EmInUpdate_EntryUpdate,
	};
private:
	CSingleMTPHttp():m_upMark(EmInUpdate_Nodef){}
public:
	~CSingleMTPHttp();

public:
	static CSingleMTPHttp& Single();

public:
	void	run(unsigned int maxCtPerLoop=10);
	void	stop();

public:
	void	addReq(THttpItem *item);

private:
	bool	isEntryUpdate() { return EmInUpdate_EntryUpdate == m_upMark;}
	void	setEntryUpdate() { m_upMark = EmInUpdate_EntryUpdate;}

public:
	void	ExcuteOne();

private:
	httpItemQueue				m_taskQ;
	httpItemQueue				m_callbackQ;

	boost::mutex				m_mutex;
	boost::condition_variable	m_cond;

	EmInUpdate					m_upMark;
};

class  CZtyMd5
{
protected:
	CZtyMd5();
	~CZtyMd5();

public:
	static CZtyMd5& Single();

	bool start(unsigned threadCount);
	void stop(void);

	void addReq(CFileMD5 *item);
	void runTime(int cnt=10);

	unsigned itemInQ(void);

private:
	TaskQueue m_md5;
};

#endif // !CHTTPMG_H_