#ifndef SOCKETSTREAM_HPP
#define SOCKETSTREAM_HPP

#include <iostream>
#include <string>

#include "type_socket.hpp"
#include "ipaddress.hpp"
#include "socket_info.hpp"
#include "socketbuf.hpp"

class socketstream : public std::iostream {
public:
    explicit socketstream(socket_info si);
    explicit socketstream(IPAddress& ip, int port, bool open_now=true);
    explicit socketstream(int d, int c, int b, int a, int port,
        bool open_now=true);
    explicit socketstream(unsigned int ip, int port, bool open_now=true);
    explicit socketstream(std::string hostname, int port, bool open_now=true);
    socketstream(const socketstream&);
    ~socketstream();

    socketstream& operator=(const socketstream&);
private:
    void init();
    basic_socket* socket = NULL;
    socketbuf* buffer;
    bool is_copy = false;
};

class bitbucket : public std::streambuf {
public:
    int_type underflow() {return 1;}
    int_type overflow(int_type ch) {return 1;}
    int sync() {return 1;}
};

#endif
