# -*- makefile -*-
TARGET=scheduler
SOURCES=main.cpp schedulerserver.cpp serverqueue.cpp

include ../config.mk

.SUFFIXES: .cpp .o

main.o:
	$(CC) $(CFLAGS) $(INCLUDEPATH) $(DEFINES) -fPIC -c main.cpp

schedulerserver.o:
	$(CC) $(CFLAGS) $(INCLUDEPATH) $(DEFINES) -fPIC -c schedulerserver.cpp schedulerserver.h

serverqueue.o:
	$(CC) $(CFLAGS) $(INCLUDEPATH) $(DEFINES) -fPIC -c serverqueue.cpp  serverqueue.h

$(TARGET): $(OBJECTS)
	mkdir -p $(DESTDIR)/bin
	$(CC) $(CFLAGS) $(INCLUDEPATH) $(DEFINES) -o $(DESTDIR)/bin/$(TARGET) $(OBJECTS) $(LDFLAGS)

clean:
	rm -rf $(BUILD)/bin/$(TARGET) *.o *.d *.gcov *.gcda *.gcno
