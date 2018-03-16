#ifndef REACTOR_NETWORK_EVENTDEMULTIPLEXER_H_
#define REACTOR_NETWORK_EVENTDEMULTIPLEXER_H_

namespace reactor
{
namespace network
{

class EventDemultiplexer
{
    public:
        EventDemultiplexer() {}
        virtual ~EventDemultiplexer() {}
        virtual int init() = 0;
        virtual int reigster_event(int fd, int type) = 0;
        virtual int unreigster_event(int fd, int type) = 0;
        virtual int modify_event(int fd, int type) = 0;
        virtual int resize(int target_size) = 0;
        virtual int loop(int timeout) = 0;
        virtual void destroy() = 0;
        EventDemultiplexer &operator=(const EventDemultiplexer &) = delete;
        EventDemultiplexer(const EventDemultiplexer &) = delete;
};

}
}

#endif
