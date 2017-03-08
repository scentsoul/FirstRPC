/*************************************************************************
	> File Name: event_loop_thread_pool.h
	> Author:eval 
	> Mail:eval@xiyoulinux.org 
	> Created Time: 2017年01月07日 星期六 11时37分20秒
 ************************************************************************/

#ifndef _EVENT_LOOP_THREAD_POOL_H
#define _EVENT_LOOP_THREAD_POOL_H
#include <map>
#include <vector>
#include <thread>
#include <memory>

namespace eylib{
    class LoopThreadPool{
    public:
        LoopThreadPool(int number);
        ~LoopThreadPool();
        int  getNextLoop(); //返回下一个loop的eventfd;
        void start(); //开始运行loop线程池
        int createEventFd(); //创建eventfd;
        static void threadFunc(int eventfd,LoopThreadPool* LoopThreadPool); //线程函数,用于创建event_loop;

    private:
        int loopNumber_;  //loop_thread的总数
        int next_; //下一个要执行任务的loop;
        std::map<int,int> fdMap_; //用于保存整个与每个loop的eventfd的映射
        std::vector<std::shared_ptr<std::thread>> threadVector_; //保存新创建线程的对象的智能指针.
    };
}
#endif
