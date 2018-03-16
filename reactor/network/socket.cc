#include "socket.h"

namespace reactor
{
namespace network
{

Address::Address(): ip_(""), port_(-1), proto_(0)
{
}

Address::~Address()
{
}

void Address::init(const string &ip, short port, int portocol)
{
    this->ip_ = ip;
    this->port_ = port;
    this->proto_ = protocol;
    this->addr_.sin_addr.s_addr = inet_addr(ip.c_str());
    this->addr_.sin_port = htons(port);
    this->addr_.sin_family = AF_INET;
}

Socket::Socket(): fd_(-1)
{
}

Socket::~Socket()
{
}

void Socket::set_fd(int fd)
{
    this->fd_ = fd;
} 

int Socket::set_no_blocking()
{
    int ret = ::fcntl(this->fd_, F_SETFL, O_NONBLOCK);
    return ret;
}

int Socket::set_reuse()
{
    int reuse = 1;
    int ret = ::setsockopt(this->fd_, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    return ret;
}

void Socket::close()
{
    if (-1 != this->fd_)
    {
        ::close(this->fd_);
        this->fd_ = -1;
    }
}

int Socket::socket(const Address &address)
{
    int ret = ::socket(AF_INET, address.proto_, 0);
    return ret;
}

int Socket::bind(const Address &address)
{
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(address.ip_.c_str());
    addr.sin_port = htons(address.port_);
    return ::bind(this->fd_, addr, sizeof(addr));
}

int Socket::listen(int backlog)
{
    return ::listen(this->fd_, backlog);
}

int Socket::accept(int &fd, Address &address)
{
    //int fd = ::accept(this->fd_
} 

}
}
