#include "ipaddress.hpp"
#include "socket_info.hpp"

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

IPAddress socket_info::getIPAddress() const {
    return ipaddress;
}

int socket_info::getPort() const {
    return port;
}

bool socket_info::getOpenNow() const {
    return open_now;
}

socket_info::socket_info(const socket_info& si) {
    this->operator=(si);
}

socket_info& socket_info::operator=(const socket_info& si) {
    ipaddress = si.getIPAddress();
    port = si.getPort();
    open_now = si.getOpenNow();
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
