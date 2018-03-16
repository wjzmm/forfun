#include "epoll_demultiplexer.h"

namespace reactor
{
namespace network
{

EpollDemultiplexer::EpollDemultiplexer():epoll_fd_(0), ptr_event_(nullptr), 
    loop_size_(0), loop_(nullptr) 
{
}

EpollDemultiplexer::EpollDemultiplexer(EventLoop *loop):epoll_fd_(0),
    ptr_event_(nullptr), loop_size_(0)
{
    loop_.reset(loop);
}

EpollDemultiplexer::~EpollDemultiplexer()
{
    this->destroy();
}

int EpollDemultiplexer::init()
{
    int ret = 0;
    ptr_event_ = static_cast<struct epoll_event *>(malloc(sizeof(struct epoll_event) * kEpollInitSize));
    if (nullptr == ptr_event_)
    {
        ret = -1;
        return ret;
    }

    loop_size_ = kEpollInitSize;
    epoll_fd_ = epoll_create(kEpollInitSize);
    if (-1 == epoll_fd_)
    {
        ret = -2;
        free(ptr_event_);
        return ret;
    }
    return ret;
}

int EpollDemultiplexer::reigster_event(int fd, int type)
{
    int ret = 0;
    struct epoll_event event;
    event.events = 0;
    event.data.fd = fd;

    if (type & kReadEvent)
    {
        event.events |= EPOLLIN;
    }
    if (type & kWriteEvent)
    {
        event.events |= EPOLLOUT;
    }

    ret = epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, fd, &event);
    if (-1 == ret)
    {
        //log
    }
    return ret;
}

int EpollDemultiplexer::unreigster_event(int fd, int type)
{
    int ret = 0;
    struct epoll_event event;
    event.events = 0;
    event.data.fd = fd;

    if (type & kReadEvent)
    {
        event.events |= EPOLLIN;
    }
    if (type & kWriteEvent)
    {
        event.events |= EPOLLOUT;
    }
     
    ret = epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, fd, &event);
    if (-1 == ret)
    {
        //log
    }
    return ret;
}

int EpollDemultiplexer::modify_event(int fd, int type)
{
    int ret = 0;
    struct epoll_event event;
    event.events = 0;
    event.data.fd = fd;

    if (type & kReadEvent)
    {
        event.events |= EPOLLIN;
    }
    if (type & kWriteEvent)
    {
        event.events |= EPOLLOUT;
    }

    ret = epoll_ctl(epoll_fd_, EPOLL_CTL_MOD, fd, &event);
    if (-1 == ret)
    {
        //log
    }
    return ret;
}

int EpollDemultiplexer::resize(int target_size)
{
    int ret = 0;
    struct epoll_event *ptr_event = nullptr;
    ptr_event = static_cast<struct epoll_event *>(realloc(ptr_event_, sizeof(struct epoll_event) * target_size));
    if (nullptr == ptr_event)
    {
        ret = -1;
        //log
        return ret;
    }
    ptr_event_ = ptr_event;
    loop_size_ = target_size;
    return ret;
}

int EpollDemultiplexer::loop(int timeout)
{
    int ret = 0;
    int events_count = 0;
    events_count = epoll_wait(epoll_fd_, ptr_event_, loop_size_, timeout);
    if (-1 == events_count)
    {
        //log
        return -1;
    }
    
    for (int64_t i = 0; i < events_count; ++i)
    {
        int event_type = 0;

        if (ptr_event_[i].events & EPOLLERR)
        {
            event_type |= kErrorEvent;
        }
        if (ptr_event_[i].events & (EPOLLIN | EPOLLHUB))
        {
            event_type |= kReadEvent;
        }
        if (ptr_event_[i].events & EPOLLOUT)
        {
            event_type |= kWriteEvent;
        }
        //增加就绪事件
        loop_.add_ready_event(ptr_event_[i].data.fd, event_type);
    }
        
    //如果事件数量超过当前epoll size,重置epoll size 扩大至两倍
    if (events_count == loop_size_)
    {
        if (resize(loop_size_ * 2) != 0)
        {
            //log
        } 
    } 
    return ret;
}

void EpollDemultiplexer::destroy()
{
    if (nullptr != ptr_event_)
    {
        free(ptr_event_);
        ptr_event_ = nullptr;
    }
}

}
}
