/*************************************************************************
	> File Name: Event_loop.h
	> Author:eval 
	> Mail:eval@xiyoulinux.org 
	> Created Time: 2017年01月07日 星期六 10时27分56秒
 ************************************************************************/

#ifndef _EVENT_LOOP_H
#define _EVENT_LOOP_H
#include <memory> 
#include <sys/epoll.h>
#include "Epoll.h"
#include "Connection.h"
#include "ObjectPool.h"
#include <stdio.h>
#include <map>
namespace eylib{
    class EventLoop{
    public:
        EventLoop(int eventFd);
        ~EventLoop();
        void loop();
        void handleEventFdRead(void);
        void handleRead(int fd);
        void handleClose(int fd);
        }
        private:
            typedef std::map<int,std::shared_ptr<Connection>> ConnectionMap;
            
            int eventFd_; //用于主loop向其他loop传递新连接的文件描述符
            std::shared_ptr<Epoll> epoll_; //所拥有的epoll循环
            struct epoll_event events_[1024]; //获取epoll的就绪事件
            ConnectionMap connectionMap_;
    };
}
#endif
