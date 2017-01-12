#include <iostream>
#include <streambuf>
#include <string>

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

class basic_socket {
public:
	
	explicit basic_socket(IPAddress& ip, int port, bool open_now=true);
	explicit basic_socket(byte d, byte c, byte b, byte a, int port, bool open_now=true);
	explicit basic_socket(unsigned int ip, int port, bool open_now=true);
	explicit basic_socket(std::string hostname, int port, bool open_now=true);

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
basic_socket::basic_socket(IPAddress& ip, int port, bool open_now) :
	ipaddress(ip), port(port), is_open(open_now) {}

/* Constructor */
basic_socket::basic_socket(byte d, byte c, byte b, byte a, int port, bool open_now) :
	ipaddress(d, c, b, a), port(port), is_open(open_now) {}

/* Constructor */

basic_socket::basic_socket(unsigned int ip, int port, bool open_now) :
	ipaddress(ip), port(port), is_open(open_now) {}

/* Constructor */
basic_socket::basic_socket(std::string hostname, int port, bool open_now) :
	ipaddress(hostname), port(port), is_open(open_now) {}

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
	explicit unix_socket(IPAddress& ip, int port, bool open_now=true) : basic_socket(ip, port, open_now) { init(); }
	explicit unix_socket(int d, int c, int b, int a, int port, bool open_now=true) : basic_socket(d, c, b, a, port, open_now) { init(); }
	explicit unix_socket(unsigned int ip, int port, bool open_now=true) : basic_socket(ip, port, open_now) { init(); }
	explicit unix_socket(std::string hostname, int port, bool open_now=true) : basic_socket(hostname, port, open_now) { init(); }

	~unix_socket() {
		if (is_open) {
			this->close();
		}
	}

	virtual void connect() {
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
	    socketAddr.sin_port = htons(port);	// Port number - convert to network byte order
	    socketAddr.sin_addr.s_addr = htonl(intAddr);	// IPv4 address (already in 
	    							// network byte order)

	    // Attempt to connect to server
	    if(::connect(fd, (struct sockaddr*)&socketAddr, sizeof(socketAddr)) < 0) {
		perror("Error connecting");
		exit(1);
	    }
	}

	virtual void close() {
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



class socketbuf {
public:
	socketbuf(basic_socket& sock);
	~socketbuf();
};
