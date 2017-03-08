/*************************************************************************
	> File Name: Base_loop.h
	> Author:eval 
	> Mail:eval@xiyoulinux.org 
	> Created Time: 2017年01月06日 星期五 22时38分41秒
 ************************************************************************/

#ifndef _BASE_LOOP_H
#define _BASE_LOOP_H
#include "Event_loop_thread_pool.h"
#include <string>
#include <memory>
namespace eylib{
    class BaseLoop{
    public:
        BaseLoop(std::string ip,int port,int loopNumber); //构造传参确定开多少个loop;
        ~BaseLoop();
        int createListenFd(void); //创建监听套接字.
        void start(void); //开始监听.
        void connectionHandle(int connfd);

    private:
        std::string ip_; //监听ip;
        int port_; //监听端口
        int listenFd_; //监听套接字.
        std::shared_ptr<LoopThreadPool> loopThreadPoolPtr_; //线程池对象指针
    };
}
#endif
