#ifndef REACTOR_NETWORK_EPOLLDEMULTIPLEXER_H_
#define REACTOR_NETWORK_EPOLLDEMULTIPLEXER_H_

#include "event_demultiplexer.h"

namespace reactor
{
namespace network
{

const int kEpollInitSize = 1024;
class EventLoop;

class EpollDemultiplexer
{
    public:
        explicit EpollDemultiplexer(EventLoop *loop);
        ~EpollDemultiplexer();
        virtual int init();
        virtual int reigster_event(int fd, int type);
        virtual int unreigster_event(int fd, int type);
        virtual int modify_event(int fd, int type);
        virtual int resize(int target_size);
        virtual int loop(int timeout);
        virtual void destroy();
        
    private:
        int epoll_fd_;
        struct epoll_event *ptr_event_;
        int loop_size_;
        std::unique_ptr<EventLoop> loop_;
};

}
}
#endif
