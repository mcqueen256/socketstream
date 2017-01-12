#ifndef SOCKETBUF_HPP
#define SOCKETBUF_HPP

#include <streambuf>
#include <string>
#include <vector>

#include "type_socket.hpp"

class socketbuf : public std::streambuf {
public:
    socketbuf(basic_socket* socket, std::size_t buffer_size=256, std::size_t put_back_size=8);
    ~socketbuf();

private:
    int_type underflow();
    int_type overflow(int_type ch);
    int sync();

    /* copy tor and assignment not implemented; */
    /* copying not allowed */
    socketbuf(const socketbuf&);
    socketbuf& operator=(const socketbuf&);
private:
    std::size_t put_back_size;
    std::vector<char> in_buffer;
    std::vector<char> out_buffer;
    basic_socket* socket;
};

#endif
