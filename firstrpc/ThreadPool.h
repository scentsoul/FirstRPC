/*************************************************************************
	> File Name: ThreadPOll.h
	> Author:Eval 
	> Mail:1040460587@qq.com 
	> Created Time: 2017年03月04日 星期六 22时00分19秒
 ************************************************************************/

#ifndef _THREADPOoL_H
#define _THREADPOoL_H
#include<list>
#include<thread>
#include<functional>
#include<memory>
#include<atomic>
#include"SyncQueue.h"

const int MaxTaskCount = 100;
class ThreadPool
{
public:
    using Task = std::functional<void()>;

    /*可供调用的接口*/
    ThreadPool(int numTthreads = std::thread::hardware_concurrency()):m_queue(MaxTaskCount){
        Start(numTthreads);
    }
    ~ThreadPool()
    {
        /*如果没有停止则主动停止线程池*/
        Stop();
    }
    void Stop()
    {
        /*保证多线程情况下只调用一次StopThreadGroup*/
        std::call_once(m_flag, [this]{StopThreadGroup();}])
    }
    void AddTask(Task&& task)
    {
        m_queue.Put(std::forward(<Task> (task));
    }
    void AddTask(const Task& Task)
    {
        m_queue.Put(Task);
                        
    }

/*线程池自身内部使用的接口 不可见*/
private:
    void Start(int numTthreads)
    {
        m_running = true;
        /*创建线程组*/
        for(int i = 0; i<numTthreads; ++i){

            /*这里make_shared函数的第一个参数指就是线程函数*/
            m_threadgroup.push_back(std::make_shared<std::thread>(&ThreadPool::RunInThread, this));
        }
    }
    void RunInThread()
    {
        while(m_running)
        {
            /*定义一个list保存从队列取的任务*/
            std::list<Task> list;
            /*取任务*/
            m_queue.Take(list);

            /*执行队列里的任务们*/
            for(auto& task:list)
            {
                if(!m_running){
                    return;
                }

                /*这里的task是functional调用对象类似于函数指针
                * 直接在后面加()就能执行它指向的函数了*/
                task();
            }

        }
    }

    void StopThreadGroup()
    {
        m_queue.Stop();     /*让同步队列中的线程停止*/
        m_running = false;

        for(auto thread:m_threadgroup){
            if(thread){
                thread->join();
            }
        }
        m_threadgroup.clear();
    }

private:
    std::list<std::shared_ptr<std::thread>> m_threadgroup;  /*处理任务的线程组*/
    SyncQueue<Task> m_queue;                                 /*同步队列*/
    atomoc_bool m_running;                                  /*是否停止的标志*/
    std::once_flag m_flag;                                   

}


#endif
