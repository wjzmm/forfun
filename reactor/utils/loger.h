#ifndef REACTOR_UTIL_LOGER_H_
#define REACTOR_UTIL_LOGER_H_

#include <string>

namespace reactor
{

namespace util
{

class Logger
{
    public:
        Logger();
        static Logger &get_instance();
        void log();
}

}
}
#endif
