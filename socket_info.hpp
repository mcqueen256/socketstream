#ifndef SOCKET_INFO_HPP
#define SOCKET_INFO_HPP

#include "ipaddress.hpp"

class socket_info;

std::ostream& operator<<(std::ostream&, const socket_info&);

class socket_info {
public:
    explicit socket_info(IPAddress& ip, int port, bool open_now=true);
    explicit socket_info(int d, int c, int b, int a, int port,
        bool open_now=true);
    explicit socket_info(unsigned int ip, int port, bool open_now=true);
    explicit socket_info(std::string hostname, int port, bool open_now=true);

    IPAddress getIPAddress() const;
    int getPort() const;
    bool getOpenNow() const;

    socket_info(const socket_info&);
    socket_info& operator=(const socket_info&);

    friend std::ostream& operator<<(std::ostream&, const socket_info&);
private:
    IPAddress ipaddress;
    int port;
    bool open_now;
};

#endif
