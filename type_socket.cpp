
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "type_socket.hpp"
#include "ipaddress.hpp"
#include "socket_info.hpp"
#include "type_socket.hpp"
#include "windows.hpp"

/* Constructor */
basic_socket::basic_socket(socket_info si) :
ipaddress(si.getIPAddress()), port(si.getPort()), is_open(si.getOpenNow()) {}

bool basic_socket::isOpen() { return is_open; }

void basic_socket::init() {
    if (is_open) {
        this->connect();
    }
}

#if WINDOWS()

// windos socket...

#else

unix_socket::unix_socket(socket_info si) : basic_socket(si) {
	init();
}

unix_socket::~unix_socket() {
    if (is_open) {
        this->close();
    }
}

void unix_socket::connect() {
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

void unix_socket::close() {
    ::close(fd);
}

std::size_t unix_socket::read(const char* buffer, std::size_t length) {
    return ::read(fd, (void*)buffer, length);
}

std::size_t unix_socket::write(const char* buffer, std::size_t length) {
    return ::write(fd, (void*)buffer, length);
}

#endif