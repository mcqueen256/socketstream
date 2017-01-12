#include <iostream>
#include <streambuf>
#include <string>
#include <vector>

#include "ipaddress.hpp"
#include "types.hpp"
#include "windows.hpp"

#if WINDOWS()
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h> 
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#endif
class socket_info;

std::ostream& operator<<(std::ostream&, const socket_info&);

class socket_info {
public:
    explicit socket_info(IPAddress& ip, int port, bool open_now=true);
    explicit socket_info(int d, int c, int b, int a, int port,
        bool open_now=true);
    explicit socket_info(unsigned int ip, int port, bool open_now=true);
    explicit socket_info(std::string hostname, int port, bool open_now=true);

    IPAddress getIPAddress();
    int getPort();
    bool getOpenNow();

    socket_info(const socket_info&);
    socket_info& operator=(const socket_info&);

    friend std::ostream& operator<<(std::ostream&, const socket_info&);
private:
    IPAddress ipaddress;
    int port;
    bool open_now;
};

/* Constructor */
socket_info::socket_info(IPAddress& ip, int port, bool open_now) :
    ipaddress(ip), port(port), open_now(open_now) {}

/* Constructor */
socket_info::socket_info(int d, int c, int b, int a, int port,
    bool open_now) :
    ipaddress(d, c, b, a), port(port), open_now(open_now) {}

/* Constructor */
socket_info::socket_info(unsigned int ip, int port, bool open_now) :
    ipaddress(ip), port(port), open_now(open_now) {}

/* Constructor */
socket_info::socket_info(std::string hostname, int port, bool open_now) :
    ipaddress(hostname), port(port), open_now(open_now) {}

IPAddress socket_info::getIPAddress() {
    return ipaddress;
}

int socket_info::getPort() {
    return port;
}

bool socket_info::getOpenNow() {
    return open_now;
}

socket_info::socket_info(const socket_info& si) {
    this->operator=(si);
}

socket_info& socket_info::operator=(const socket_info& si) {
    ipaddress = ((socket_info)si).getIPAddress();
    port = ((socket_info)si).getPort();
    open_now = ((socket_info)si).getOpenNow();
    return *this;
}

std::ostream& operator<<(std::ostream& os, const socket_info& s) {
    os << s.ipaddress.getByte4() << '.'
        << s.ipaddress.getByte3() << '.'
        << s.ipaddress.getByte2() << '.'
        << s.ipaddress.getByte1() << ':'
        << s.port;
    return os;
}


class basic_socket {
public:
    
    explicit basic_socket(socket_info si);
    virtual ~basic_socket() {};

    /* Methods that must be overrided. */
    virtual void connect() = 0;
    virtual void close() = 0;
    virtual std::size_t read(const char* buffer, std::size_t length) = 0;
    virtual std::size_t write(const char* buffer, std::size_t length) = 0;

    /* Status methods */
    bool isOpen();

    /* Copy ctor and assignment not implemented; */
    /* copying not allowed */
    basic_socket(const basic_socket &);
    basic_socket& operator=(const basic_socket &);

protected:
    void init();

protected:
    bool is_open = false;
    IPAddress ipaddress;
    int port;
};

/* Constructor */
basic_socket::basic_socket(socket_info si) :
    ipaddress(si.getIPAddress()), port(si.getPort()), is_open(si.getOpenNow()){}

bool basic_socket::isOpen() { return is_open; }

void basic_socket::init() {
    if (is_open) {
        this->connect();
    }
}

#if WINDOWS()
#else

class unix_socket : public basic_socket {
public:
    explicit unix_socket(socket_info si) : basic_socket(si) { init(); }
    ~unix_socket() {
        if (is_open) {
            this->close();
        }
    }

    void connect() {
        std::cout << "unix_socket::connect()" << std::endl;
        struct sockaddr_in socketAddr;
        fd = ::socket(AF_INET, SOCK_STREAM, 0);
        if (fd < 0) {
            //TODO: throw error
        }

        uint32_t intAddr  = ipaddress.getByte4()<<24;
                 intAddr += ipaddress.getByte3()<<16;
                 intAddr += ipaddress.getByte2()<<8;
                 intAddr += ipaddress.getByte1();

        socketAddr.sin_family = AF_INET;  // IP v4
        socketAddr.sin_port = htons(port);  // Port number - convert to network byte order
        socketAddr.sin_addr.s_addr = htonl(intAddr);    // IPv4 address (already in 
                                    // network byte order)

        // Attempt to connect to server
        if(::connect(fd, (struct sockaddr*)&socketAddr, sizeof(socketAddr)) < 0) {
        perror("Error connecting");
        exit(1);
        }
    }

    void close() {
        std::cout << "unix_socket::close()" << std::endl;
        ::close(fd);
    }

    std::size_t read(const char* buffer, std::size_t length) {
        return ::read(fd, (void*)buffer, length);
    }

    std::size_t write(const char* buffer, std::size_t length) {
        return ::write(fd, (void*)buffer, length);
    }
private:
    int fd;
};

#endif

#if WINDOWS()
typedef win_socket type_socket;
#else
typedef unix_socket type_socket;
#endif

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

class socketstream : public std::iostream {
public:
    socketstream(socket_info si);
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
    basic_socket* socket;
    bool is_copy = false;
};

/*
 * Main constructor. 
 */
socketstream::socketstream(socket_info si) : socket(new type_socket(si)), std::iostream(new socketbuf(socket)) {
}

/* Shorthand constructors */
socketstream::socketstream(IPAddress& ip, int port,
    bool open_now) : socketstream(socket_info(ip, port, open_now)) {}
socketstream::socketstream(int d, int c, int b, int a, int port,
    bool open_now) :
    socketstream(socket_info(d, c, b, a, port, open_now)) {}
socketstream::socketstream(unsigned int ip, int port,
    bool open_now) : socketstream(socket_info(ip, port, open_now)) {}
socketstream::socketstream(std::string hostname, int port,
    bool open_now) : socketstream(socket_info(hostname, port, open_now)) {}

/* Clone constructor */
socketstream::socketstream(const socketstream& ss) : std::iostream(NULL) {
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



