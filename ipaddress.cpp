#include "ipaddress.hpp"
#include "types.hpp"

/*
 * Basic constructor
 * 'd' is the MSByte, 'a' is the LSByte.
 */
IPAddress::IPAddress(byte d, byte c, byte b, byte a) :
	b1(a), b2(b), b3(c), b4(d) {}

/* Deconstructor */
IPAddress::~IPAddress() {}

/*
 * Constructor
 * Copies the values of the 'other' IPAddress to this IPAddress.
 *
 * @param other is an integer representation of an IPAddress from which the
 * values will be copied from.
 */
IPAddress::IPAddress(const int& other) {
	b4 = (other>>24) & 0xFF;
	b3 = (other>>16) & 0xFF;
	b2 = (other>>8) & 0xFF;
	b1 = other & 0xFF;
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
IPAddress::IPAddress& operator=(const IPAddress& other) {
	b1 = other.byte1();
	b2 = other.byte2();
	b3 = other.byte3();
	b4 = other.byte4();
	return *this;
}

/* Returns the individual bytes of the IPAddress. MSByte is b4, LSByte is b1. */
byte IPAddress::getByte1() { return b1; }
byte IPAddress::getByte2() { return b2; }
byte IPAddress::getByte3() { return b3; }
byte IPAddress::getByte4() { return b4; }