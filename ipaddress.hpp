#ifndef IPADDRESS_HPP
#define IPADDRESS_HPP

#include <ostream>
#include <string>

#include "types.hpp"

class IPAddress;

std::ostream& operator<<(std::ostream&, const IPAddress&);

class IPAddress {
public:
	IPAddress(int a, int b, int c, int d);
	~IPAddress();

	IPAddress();
	IPAddress(const int &);
	IPAddress(const std::string &);
	IPAddress(const IPAddress &);

	int getByte1() const noexcept;
	int getByte2() const noexcept;
	int getByte3() const noexcept;
	int getByte4() const noexcept;

	IPAddress& operator=(const IPAddress&);
	friend std::ostream& operator<<(std::ostream&, const IPAddress&);

private:
	int b1=0, b2=0, b3=0, b4=0;
};

#endif