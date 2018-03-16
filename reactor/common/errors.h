#ifndef REACTOR_COMMON_ERRORS_H_
#define REACTOR_COMMON_ERRORS_H_

namespace reactor
{
namespace common
{

const int kSucc = 0;
const int kErrEpoll = -1001;
const int kErrBind = -1002;
const int kErrListen = -1003;
const int kErrAccept = -1004;

}
}
#endif
