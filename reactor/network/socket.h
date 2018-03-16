#ifndef NETWORK_SOCKET_H_
#define NETWORK_SOCKET_H_

#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

namespace reactor
{
namespace network
{

class Address
{
    public:
        Address();
        ~Address();
        string get_ip() const { return this->ip_; }
        short get_port() const { return this->port_; }
        int get_protocol() const { return this->proto_; }
        const struct sockaddr_in &get_addr() const { return this->addr_; }
        void init(const string &ip, short port, int protocol); 

    private:
        string ip_;
        short port_;
        int proto_;
        struct sockaddr_in addr_;

};


class Socket
{
    public:
        Socket();
        explicit Socket(int fd);
        ~Socket();

        void set_fd(int fd);
        int get_fd() const { return fd_; }
        int set_no_blocking();
        int set_reuse();
        void close();
        int socket(const Address &address);
        int bind(const Address &address);
        int listen(int backlog);
        int accept(int &fd, Address &address);

        Socket &operator=(const Socket&) = delete;
        Socket(const Socket&) = delete;

    private:
        int fd_;
};

ssize_t send(int fd, const void *buf, size_t nbytes);
ssize_t recv(int fd, void *buf, size_t nbytes);

}
}

#endif
