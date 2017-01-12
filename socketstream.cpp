#include <iostream>
#include <string>

#include "ipaddress.hpp"
#include "socket_info.hpp"
#include "socketstream.hpp"

/*
 * Main constructor. 
 */

socketstream::socketstream(socket_info si) : std::iostream(new bitbucket) {
    socket = new type_socket(si);
    buffer = new socketbuf(socket);
    rdbuf(buffer);

}

/* Shorthand constructors */
socketstream::socketstream(IPAddress& ip, int port, bool open_now) : socketstream(socket_info(ip, port, open_now)) {}
socketstream::socketstream(int d, int c, int b, int a, int port, bool open_now) : socketstream(socket_info(d, c, b, a, port, open_now)) {}
socketstream::socketstream(unsigned int ip, int port, bool open_now) : socketstream(socket_info(ip, port, open_now)) {}
socketstream::socketstream(std::string hostname, int port, bool open_now) : socketstream(socket_info(hostname, port, open_now)) {}

/* Clone constructor */
socketstream::socketstream(const socketstream& ss) : std::iostream(ss.rdbuf()) {
    is_copy = true;
    this->rdbuf(ss.rdbuf());
    this->socket = NULL;
}

socketstream::~socketstream() {
    if (!is_copy) {
        delete this->rdbuf();
        delete socket; 
    }
}



