#include "event_loop.h"

namespace reactor
{
namespace network
{

EventLoop::EventLoop(): pooler_(nullptr), reigstered_events_(),
    ready_event_list_(), is_running_(true)
{
}

EventLoop::~EventLoop()
{
}

int EventLoop::init(EpollDemultiplexer *poller)
{
    int ret = poller->init();
    if (ret != 0)
    {
        //log
    }
    pooler_.reset(poller);
    return ret;
}

int EventLoop::register_event(std::shared_ptr<Event> event)
{
    int ret = 0;
    int type = event->type;
    if ((type & kReadEvent) || (type & kWriteEvent))
    {
        ret = poller_->register_event(event->fd, event->type);
        if (ret != 0)
        {
            //log
            return ret;
        }
        else
        {
            registered_events[event->fd] = event;
        }
    }

    return ret;
}

int EventLoop::unregister_event(std::shared_ptr<Event> event)
{
    int ret = 0;
    int type = event->type;
    if ((type & kReadEvent) || (type & kWriteEvent))
    {
        ret = pooler_->unregister_event(event->fd, event->type);
        if (ret != 0)
        {
            //log
            return ret;
        }
        else
        {
            reigstered_events_.erase(event->fd);
        }

    }
    return ret;
}
    
int EventLoop::modify_event(std::shared_ptr<Event> event)
{
    int ret = 0;
    int type = event->type;
    if ((type & kReadEvent) || (type & kWriteEvent))
    {
        ret = pooler_->modify_event(event->fd, event->type);
        if (ret != 0)
        {
            //log
            return ret;
        }
        else
        {
            reigstered_events_[event->fd] = event;
        }

    }
    return ret;
}

int EventLoop::loop()
{
    int ret = 0;
    while (is_running_)
    {
        ret = poller_->loop(1000);
        if (ret != 0)
        {
            //log
            is_running_ = false;
            return ret;
        }

        for (auto e = ready_event_list_.cbegin(); e != ready_event_list_.cend(); ++ e)
        {
            const std::shared_ptr<event> event = registered_events_[e->fd];
            if (e->type & r->type & kReadEvent)
            {
                e->read_cb(e->fd, kReadEvent, e->args);
            }
            if (e->type & r->type & kWriteEvent)
            {
                e->write_cb(e->fd, kReadEvent, e->args);
            }
        }
        clear_ready_event();
    }
    return ret;
}

void EventLoop::destroy()
{
    poller_->destroy();
}

void EventLoop::add_ready_event(int fd, int event_type)
{
    Event event = *registered_events_[fd];
    event.type = event_type;
    ready_event_list_.push_back(event);
}

void EventLoop::clear_ready_event()
{
    ready_event_list_.clear();
}


}
}

