#include <streambuf>
#include <string>
#include <vector>

#include "socketbuf.hpp"
#include "type_socket.hpp"

socketbuf::~socketbuf() {}

socketbuf::socketbuf(basic_socket* socket, std::size_t buffer_size, std::size_t put_back_size) :
    socket(socket),
    put_back_size(put_back_size),
    in_buffer(std::max(buffer_size, put_back_size) + put_back_size),
    out_buffer(buffer_size + 1)
{
    // Setup input
    char* end = &in_buffer.front() + in_buffer.size();
    setg(end, end, end);

    // Setup output
    char *base = &out_buffer.front();
    setp(base, base + out_buffer.size() - 1);
}

std::streambuf::int_type socketbuf::underflow() {
    if (gptr() < egptr())
        return traits_type::to_int_type(*gptr());

    char* base = &in_buffer.front();
    char* start = base;

    if (eback() == base) {
        std::memmove(base, egptr() - put_back_size, put_back_size);
        start += put_back_size;
    }

    std::size_t n = socket->read(start, in_buffer.size() - (start - base));
    if (n == 0)
        return traits_type::eof();

    setg(base, start, start + n);

    return traits_type::to_int_type(*gptr());
}

/*
 * invarient offered by c++ library: assert(std::less_equal<char *>()(pptr(), epptr()));
 */
std::streambuf::int_type socketbuf::overflow(int_type ch) {
    if (socket->isOpen() && ch != traits_type::eof()) {
        *pptr() = ch;
        pbump(1);
        return ch;
    }

    return traits_type::eof();
}

int socketbuf::sync() {
    std::ptrdiff_t n = pptr() - pbase();
    pbump(-n);
    std::size_t n_written = socket->write(pbase(), n);
    if (n == 0) {
        return -1;
    }
    return 0;
}