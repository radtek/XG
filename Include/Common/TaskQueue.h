#ifndef __TASK_QUEUE__
#define __TASK_QUEUE__
#include <queue>
#include <boost/thread.hpp>
#include <boost/lockfree/queue.hpp>
#include "../../Include/allocmem/alloc_basemg.h"
#include "../../Include/Common/XDirtyMark.h"

// 向TaskQueue中添加新任务
// 通过继承TaskBase并重写run() callback()

class TaskBase : public XObjMark
{
public: 
	virtual ~TaskBase(void) {}

	// run函数为任务执行函数
	// 在TaskQueue启动的线程中执行
	virtual void run(void) = 0; 

	// callback函数负责处理run函数产生的结果
	// 通常由主线程调用
	virtual void callback(void) {}
}; 

template<typename RunFunc,typename CallbackFunc>
class TaskFuncObject : public TaskBase
{
public:
	TaskFuncObject(const RunFunc &run,const CallbackFunc &callback) 
		: run_(run),callback_(callback) {
	}

	virtual void run(void) {
		run_();
	}

	virtual void callback(void) {
		callback_();
	}

private:
	RunFunc run_;
	CallbackFunc callback_; 
};


class TaskQueue
{ 
private:
	struct TaskItem 
	{
		TaskBase *task;
		unsigned priority;
		  
		friend bool operator < (const TaskItem &left,const TaskItem &right) {
			return left.priority < right.priority;
		}
	}; 
	typedef std::priority_queue<TaskItem> Queue;

public: 
	TaskQueue(void);
	~TaskQueue(void);

	bool running(void) const;

	// 启动任务队列执行线程
	void start(unsigned threadCount);

	// 终止任务队列所有线程、等待线程结束
	// 并所有销毁未执行的任务
	void stop(void);
	
	// 向任务队列添加新的任务
	// priority数值越大，优先级越高
	void post(unsigned priority,TaskBase *task);

	// 通过模板类型向任务队列添加新的任务
	template<typename RunFunc,typename CallbackFunc>
	void postobj(unsigned priority,const RunFunc &run,const CallbackFunc &callback) { 
		post(priority,makeTask(run,callback));
	}

	// callback函数通常由主线程调用处理task->run()后产生的结果
	// callback函数调用task->callback()后会销毁task
	//		调用CSgsAllocXBlockMg::single()->delete_xx(task)
	void callback(int cnt=20);

	unsigned taskCount(void);

private:
	template<typename RunFunc,typename CallbackFunc>
	TaskBase* makeTask(const RunFunc &run,const CallbackFunc &callback) {
		return CSgsAllocXBlockMg::single()->new_2xx<TaskFuncObject<RunFunc,CallbackFunc> >(run,callback); 
	}

	void freeTask(TaskBase *task);
	void threadfunc(void);
	virtual bool enterThread(void);
	virtual void leaveThread(void);

	bool stop_;
	Queue taskQ_;

	boost::mutex mutex_;
	boost::condition_variable cond_;

	boost::thread_group threadGroup_; 
	
	boost::lockfree::queue<TaskBase*> callbackQ_;	 
};
 
#endif
