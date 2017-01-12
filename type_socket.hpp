#ifndef TYPE_SOCKET_HPP
#define TYPE_SOCKET_HPP

#include <string>

#include "ipaddress.hpp"
#include "socket_info.hpp"
#include "windows.hpp"

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

#if WINDOWS()

class win_socket : public basic_socket {
// TODO: complete this one.
};

#else

class unix_socket : public basic_socket {
public:
    explicit unix_socket(socket_info si);
    ~unix_socket();
    
    void connect();
    void close();
    std::size_t read(const char* buffer, std::size_t length);
    std::size_t write(const char* buffer, std::size_t length);
private:
    int fd;
};

#endif

#if WINDOWS()
typedef win_socket type_socket;
#else
typedef unix_socket type_socket;
#endif

#endif