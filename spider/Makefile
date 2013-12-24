# -*- makefile -*-
TARGET:=spider

HEADERS=spider.h servermanager.h
SOURCES=spider.cpp servermanager.cpp main.cpp

include ../config.mk

LIBS+=-lsmbclient -lmysqlpp -lmysqlclient -ldata_storage -lmagic

.SUFFIXES: .cpp .o

main.o:
	$(CC) $(CFLAGS) $(INCLUDEPATH) $(DEFINES) -fPIC -c main.cpp

spider.o:
	$(CC) $(CFLAGS) $(INCLUDEPATH) $(DEFINES) -fPIC -c spider.cpp spider.h

servermanager.o:
	$(CC) $(CFLAGS) $(INCLUDEPATH) $(DEFINES) -fPIC -c servermanager.cpp servermanager.h

$(TARGET): $(OBJECTS)
	mkdir -p $(DESTDIR)/bin
	$(CC) $(CFLAGS) $(INCLUDEPATH) $(DEFINES) -o $(DESTDIR)/bin/spider $(OBJECTS) $(LIBS)

clean:
	rm -rf $(DESTDIR)/bin/spider *.o *.d *.gcov *.gcda *.gcno
