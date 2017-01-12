#include <iostream>
#include <streambuf>
#include <string>

#include "ipaddress.hpp"
#include "types.hpp"

class basic_socket {
public:
	explicit basic_socket(IPAddress& ip, word port, bool open_now=true);
	explicit basic_socket(byte a, byte b, byte c, byte d, word port, bool open_now=true);
	explicit basic_socket(unsigned int ip, word port, bool open_now=true);
	explicit basic_socket(std::string hostname, word port, bool open_now=true);
	~basic_socket();

    /* Methods that must be overrided. */
	virtual bool connect() = 0;
	virtual bool close() = 0;
	virtual std::size_t read(const char* buffer, std::size_t length) = 0;
	virtual std::size_t write(const char* buffer, std::size_t length) = 0;

    /* Status methods */
	bool isOpen();

	/* Copy ctor and assignment not implemented; */
	/* copying not allowed */
	basic_socket(const basic_socket &);
	basic_socket& operator=(const basic_socket &);

private:
	void init();

private:
	bool is_open = false;
	IPAddress ipaddress;
	word port;
};

/*
 * Constructor
 *
 */
basic_socket::basic_socket(IPAddress& ip, word port, bool open_now) :
	ipaddress(ip), port(port), is_open(open_now) { init(); }

/*
 * Constructor
 *
 */
basic_socket::basic_socket(byte d, byte c, byte b, byte a, word port, bool open_now) :
	ipaddress(d, c, b, a), port(port), is_open(open_now) { init(); }

/*
 * Constructor
 *
 */
basic_socket::basic_socket(unsigned int ip, word port, bool open_now) :
	ipaddress(ip), port(port), is_open(open_now) { init(); }

/*
 * Constructor
 *
 */
basic_socket::basic_socket(std::string hostname, word port, bool open_now) :
	ipaddress(hostname), port(port), is_open(open_now) { init(); }

basic_socket::~basic_socket() {}



class socketbuf {
public:
	socketbuf(basic_socket& sock);
	~socketbuf();
};