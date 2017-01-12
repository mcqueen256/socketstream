#include "ipaddress.hpp"
#include "netutil.hpp"

#include <iostream>


int main() {
	IPAddress ip = hostnameToIPAddress("windows-10.shared");
	std::cout << ip << std::endl;
	return 0;
}