#ifndef NETUTIL_HPP
#define NETUTIL_HPP

#include "ipaddress.hpp"

#include <string>

IPAddress hostnameToIPAddress(std::string hostname);

#endif