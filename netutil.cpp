#include "ipaddress.hpp"
#include "netutil.hpp"
#include "types.hpp"
#include "windows.hpp"

#include <exception>
#include <string>
#include <sstream>
#include <vector>

#if WINDOWS()
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h> 
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#endif

void split(const std::string &s, char delimiter, std::vector<std::string> &elems) {
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delimiter)) {
        elems.push_back(item);
    }
}

/*
 * Converst a hostname string to an IPAddress object.
 * 
 * @param hostname is the string domain representaion.
 * @return IPAddress of the hostname.
 * @throws std::runtime_error if the domain lookup fails.
 * @throws std::logic_error if the decoding of inet_ntoa() fails.
 */
IPAddress hostnameToIPAddress(std::string hostname) noexcept(false) {
#if WINDOWS()

#else
    struct addrinfo* addressInfo;
    if (getaddrinfo(hostname.c_str(), NULL, NULL, &addressInfo)) {
    	throw std::runtime_error("could not resolve hostname \"" + hostname + '"');
    }
	std::string addrstr = inet_ntoa(((struct sockaddr_in*)(addressInfo->ai_addr))->sin_addr);

	/* Decode string address */
	std::logic_error e("Address was not properly converted into buffer");
	std::vector<std::string> addr_vec_buf;
	split(addrstr, '.', addr_vec_buf);
	if (addr_vec_buf.size() != 4) {
		throw e;
	}
    try {
    	return IPAddress(std::stoi(addr_vec_buf[0]),
		std::stoi(addr_vec_buf[1]),
		std::stoi(addr_vec_buf[2]),
		std::stoi(addr_vec_buf[3]));
    } catch (std::invalid_argument e) {
    	throw e;
    }
	
#endif
}