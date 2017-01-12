#include "ipaddress.hpp"
#include "netutil.hpp"
#include "socket.cpp"

#include <iostream>


int main() {
	IPAddress ip = hostnameToIPAddress("windows-10.shared");
	std::cout << ip << std::endl;
	unix_socket sock(127,0,0,1, 12345);
	sock.write("hello", 5);
	sock.write("bye", 3);
	return 0;
}