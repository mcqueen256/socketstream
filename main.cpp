#include <iostream>
#include <string>
#include "socketstream.hpp"

int main() {
	socketstream ss(127,0,0,1, 12345);

	ss << "Enter text: " << std::flush;
	std::string s;
	ss >> s;

	std::cout << "captured input is: '" << s << "'" << std::endl;
	return 0;
}