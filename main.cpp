#include "ipaddress.hpp"
#include "netutil.hpp"
#include "socket.cpp"

#include <iostream>


int main() {
	IPAddress ip = hostnameToIPAddress("windows-10.shared");
	std::cout << ip << std::endl;
	unix_socket sock(127,0,0,1, 12345);
	socketbuf sbuf(sock);
	std::streambuf* orig = std::cin.rdbuf(&sbuf);
	std::string s;
	std::cin >> s;
	std::cout << "captured input is: '" << s << "'" << std::endl;

	return 0;
}