#ifndef REACTOR_NETWORK_EVENTLOOP_H_
#define REACTOR_NETWORK_EVENTLOOP_H_

namespace reactor
{
namespace network
{

const int kReadEvent = 1 << 0;
const int kWriteEvent = 1 << 1;
const int kErrorEvent = 1 << 2;

struct Event
{
    public:
        typedef std::function<int(int, int, void*)> EventCallback;
        int fd;
        int type;
        void *args;
        int64_t timeout;

        std::vector<Event*>::size_type index;
        EventCallback read_cb;
        EventCallback write_cb;
};

class EpollDemultiplexer;

class EventLoop
{
    public:
        EventLoop();
        ~EventLoop();
        int init(EpollDemultiplexer *poller);
        int register_event(std::shared_ptr<Event> event);
        int unregister_event(std::shared_ptr<Event> event);
        int modify_event(std::shared_ptr<Event> event);
        int loop();
        int destroy();
        int get_loop_size();
        void add_ready_event(int fd, int event_type);
        void clear_ready_event();
        EventLoop &operator=(const EventLoop&) = delete;
        EventLoop(const EventLoop&) = delete;

    private:
        bool is_running_;
        std::unorder_map<int, std::shared_ptr<Event> > reigstered_events_;
        std::list<Event> ready_event_list_;
        std::unique_ptr<EpollDemultiplexer> pooler_;
}

}
}
#endif
