#ifndef IPADDRESS_HPP
#define IPADDRESS_HPP

#include "types.h"

class IPAddress {
public:
	IPAddress(byte a, byte b, byte c, byte d);
	~IPAddress();

	IPAddress::IPAddress(const int &);
	IPAddress(const IPAddress &);
	IPAddress& operator=(const IPAddress &);

	byte getByte1();
	byte getByte2();
	byte getByte3();
	byte getByte4();
private:
	byte b1=0, b2=0, b3=0, b4=0;
};

#endif