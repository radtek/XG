#ifndef __TASK_QUEUE__
#define __TASK_QUEUE__
#include <queue>
#include <boost/thread.hpp>
#include <boost/lockfree/queue.hpp>
#include "../../Include/allocmem/alloc_basemg.h"
#include "../../Include/Common/XDirtyMark.h"

// ��TaskQueue�����������
// ͨ���̳�TaskBase����дrun() callback()

class TaskBase : public XObjMark
{
public: 
	virtual ~TaskBase(void) {}

	// run����Ϊ����ִ�к���
	// ��TaskQueue�������߳���ִ��
	virtual void run(void) = 0; 

	// callback����������run���������Ľ��
	// ͨ�������̵߳���
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

	// �����������ִ���߳�
	void start(unsigned threadCount);

	// ��ֹ������������̡߳��ȴ��߳̽���
	// ����������δִ�е�����
	void stop(void);
	
	// �������������µ�����
	// priority��ֵԽ�����ȼ�Խ��
	void post(unsigned priority,TaskBase *task);

	// ͨ��ģ�������������������µ�����
	template<typename RunFunc,typename CallbackFunc>
	void postobj(unsigned priority,const RunFunc &run,const CallbackFunc &callback) { 
		post(priority,makeTask(run,callback));
	}

	// callback����ͨ�������̵߳��ô���task->run()������Ľ��
	// callback��������task->callback()�������task
	//		����CSgsAllocXBlockMg::single()->delete_xx(task)
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
