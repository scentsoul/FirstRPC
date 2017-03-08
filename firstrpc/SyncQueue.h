/*************************************************************************
	> File Name: SyncQueue.h
	> Author:Eval 
	> Mail:1040460587@qq.com 
	> Created Time: 2017年03月04日 星期六 18时47分03秒
 ************************************************************************/

#ifndef _SYNCQUEUE_H
#define _SYNCQUEUE_H
#include<list>
#include<mutex>
#include<thread>
#include<condition_variable>
#include<iostream>

using namespace std;
template<typename T>
class SyncQueue
{
public:
    SyncQueue(int MaxSize):m_maxSize(MaxSize), m_needStop(false){

    }
    void Put(const T& x)
    {
        Add(x);
    }
    void Put(T&& x)
    {
        Add(std::forward<T>(x));
    }
    void Take(std::list<list>& list)
    {
        std::unique_lock<std::mutex> locker(m_mutex);
        m_notEmpty.wait(locker, [this]{return m_needStop || NotEmpty();});

        if(m_needStop){
            return ;
        }

        list = std::move(m_queue);
        m_notFull.notify_one;     /*在这个地方就交出锁了*/


        /*可以继续执行其他处理*/
    }
    void Take(T& t)
    {
        std::unique_lock<std::mutex> locker(m_mutex);
        m_notEmpty.wait(locker, [this]{return m_needStop || NotEmpty();});

        if(m_needStop){
            return ;
        }
        t = m_queue.front();
        m_queue.pop_front();
        m_notFull.notify_one(); /*在这个地方就交出锁了*/
    }
    bool Empty()
    {
        std::lock_guard<std::mutex> locker(m_mutex);
        return m_queue.empty();
    }
    bool Full()
    {
        std::lock_guard<std::mutex> locker(m_mutex);
        return m_queue.size() == MaxSize;
    }
    size_t Size()
    {
        std::lock_guard<std::mutex> locker(m_mutex);
        return m_queue.size();
    }
    int count()
    {
        return m_queue.size();
    }
    bool NotFull() const
    {
        bool full = m_queue.size() >= m_maxSize;
        if(full){
            std::cout << "缓冲区满了,需要等待..." << std::endl;
        }
        return !full;
    }
    bool NotEmpty() const
    {
        bool empty = m_queue.empty();
        if(empty){
            std::cout << "缓冲区空了,需要等待...异步层的线程ID:" << this_thread::get_id() << std::endl;
        }
        return !empty;
    }
    template<typename F>
    void Add(F&&X)
    {
        std::unique_lock<std::mutex> locker(m_mutex);
        m_notFull.wait(locker, [this]{return m_needStop||NotFull();})

        if(m_needStop){
            return;
        }
        m_queue.push_back(std::forward<F>(x));
        m_notEmpty.notify_one();
    }


private:
    std::list<T> m_queue;    /*缓冲区*/
    std::mutex m_mutex;      /*互斥量和条件变量结合起来使用*/
    std::condition_variable m_notEmpty;   /*不为空的条件变量*/
    std::condition_variable m_notFull;    /*没有满的条件变量*/

    int m_maxSize;      /*同步队列的最大size*/
    bool m_needStop;    /*停止的标志*/
};

#endif

