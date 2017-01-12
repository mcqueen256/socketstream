.PHONY: all clean

##############################################################################
#                            Project Settings                                #
##############################################################################
# Build settings
CC = g++
CC_OPTIMIZATION = -O3
CC_FLAGS = -g -std=c++11 -stdlib=libc++ -Wall -pedantic $(CC_OPTIMIZATION)

TARGET = socket_test

##############################################################################
#                              Project Build                                 #
##############################################################################
# Build the whole project
all: $(TARGET)
	@echo "Building Project... Done."

##############################################################################
#                             Object Files                                   #
##############################################################################
socket_info.o: socket_info.cpp
	$(CC) $(CC_FLAGS) -c $^

ipaddress.o: ipaddress.cpp
	$(CC) $(CC_FLAGS) -c $^

netutil.o: netutil.cpp
	$(CC) $(CC_FLAGS) -c $^

type_socket.o: type_socket.cpp
	$(CC) $(CC_FLAGS) -c $^

socketbuf.o: socketbuf.cpp
	$(CC) $(CC_FLAGS) -c $^

socketstream.o: socketstream.cpp
	$(CC) $(CC_FLAGS) -c $^

##############################################################################
#                           Project Excecutables                             #
##############################################################################
$(TARGET): main.cpp socket_info.o ipaddress.o netutil.o type_socket.o socketbuf.o socketstream.o
	$(CC) $(CC_FLAGS) -o $@ $^

##############################################################################
#                              Clean Files                                   #
##############################################################################
clean:
	rm -rf *.o $(TARGET)
	# For macos
	rm -rf *.dSYM