#include "ipaddress.hpp"

#include <ostream>
#include <string>

#include "netutil.hpp"
#include "types.hpp"

std::ostream& operator<<(std::ostream& os, const IPAddress& ip) {
	os << ip.b4 << '.' << ip.b3 << '.' << ip.b2 << '.' << ip.b1;
	return os;
}

/*
 * Basic constructor
 * 'd' is the MSByte, 'a' is the LSByte.
 */
IPAddress::IPAddress(int d, int c, int b, int a) :
	b1(a), b2(b), b3(c), b4(d) {}

/*
 * Basic constructor
 * All bytes are set to zero.
 */
IPAddress::IPAddress() :
	b1(0), b2(0), b3(0), b4(0) {}

/* Deconstructor */
IPAddress::~IPAddress() {}

/*
 * Constructor
 * Converts the integer 'address' into an IPAddress.
 *
 * @param address is an integer representation of an IPAddress from which the
 * values will be copied from.
 */
IPAddress::IPAddress(const int& address) {
	b4 = (address>>24) & 0xFF;
	b3 = (address>>16) & 0xFF;
	b2 = (address>>8) & 0xFF;
	b1 = address & 0xFF;
}

/*
 * Constructor
 * Converts the std::string value 'hostname' into an IPAddress by DNS lookup.
 *
 * @param hostname is a domain name on the network.
 */
IPAddress::IPAddress(const std::string& hostname) {
	this->operator=(hostnameToIPAddress(hostname));
}

/*
 * Constructor
 * Copies the values of the 'other' IPAddress to this IPAddress.
 *
 * @param other is the IPAddress from which the values will be copied from.
 */
IPAddress::IPAddress(const IPAddress& other) {
	this->operator=(other);
}

/*
 * Copies the values of the 'other' IPAddress to this IPAddress.
 *
 * @param other is the IPAddress from which the values will be copied from.
 * @return a reference to this IPAddress.
 */
IPAddress& IPAddress::operator=(const IPAddress& rhs) {
	this->b1 = rhs.b1;
	this->b2 = rhs.b2;
	this->b3 = rhs.b3;
	this->b4 = rhs.b4;
	return *this;
}

/* Returns the individual bytes of the IPAddress. MSByte is b4, LSByte is b1. */
int IPAddress::getByte1() const noexcept { return b1; }
int IPAddress::getByte2() const noexcept { return b2; }
int IPAddress::getByte3() const noexcept { return b3; }
int IPAddress::getByte4() const noexcept { return b4; }