# -*- makefile -*-
TARGET:=libcppsockets

HEADERS = socketaddress.h	\
		  abstractsocket.h	\
		  datasocket.h		\
		  tcplistener.h		\
		  tcpsocket.h		\
		  udpsocket.h

SOURCES = socketaddress.cpp		\
		  abstractsocket.cpp	\
		  datasocket.cpp		\
		  tcplistener.cpp		\
		  tcpsocket.cpp			\
		  udpsocket.cpp			\

include ../config.mk

.SUFFIXES: .cpp .o

socketaddress.o:
	$(CC) $(CFLAGS) $(INCLUDEPATH) $(DEFINES) -fPIC -c socketaddress.cpp  socketaddress.h

abstractsocket.o:
	$(CC) $(CFLAGS) $(INCLUDEPATH) $(DEFINES) -fPIC -c abstractsocket.cpp  abstractsocket.h

datasocket.o:
	$(CC) $(CFLAGS) $(INCLUDEPATH) $(DEFINES) -fPIC -c datasocket.cpp  datasocket.h

tcplistener.o:
	$(CC) $(CFLAGS) $(INCLUDEPATH) $(DEFINES) -fPIC -c tcplistener.cpp  tcplistener.h

tcpsocket.o:
	$(CC) $(CFLAGS) $(INCLUDEPATH) $(DEFINES) -fPIC -c tcpsocket.cpp  tcpsocket.h

udpsocket.o:
	$(CC) $(CFLAGS) $(INCLUDEPATH) $(DEFINES) -fPIC -c udpsocket.cpp  udpsocket.h

$(TARGET): $(OBJECTS)
	mkdir -p $(DESTDIR)/lib
	$(CC) $(CFLAGS) $(INCLUDEPATH) $(DEFINES) -shared -o $(DESTDIR)/lib/libcppsockets.so $(OBJECTS)

clean:
	rm -rf *.o $(DESTDIR)/lib/libcppsockets.so *.d *.gcov *.gcda *.gcno
