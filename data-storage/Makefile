# -*- makefile -*-
TARGET:=libdata_storage
SOURCES = entities.cpp
HEADERS = entities.h

include ../config.mk

LIBS+=-lmysqlclient -lmysqlpp

.SUFFIXES: .cpp .o

entities.o:
	$(CC) $(CFLAGS) $(INCLUDEPATH) $(DEFINES) -fPIC -c entities.cpp entities.h

$(TARGET): $(OBJECTS)
	mkdir -p $(DESTDIR)/lib
	$(CC) $(CFLAGS) $(INCLUDEPATH) $(DEFINES) -shared -o $(DESTDIR)/lib/libdata_storage.so $(OBJECTS) $(LIBS)

clean:
	rm -rf *.o $(DESRDIR)/lib/libdata_storage.so *.d *.gcov *.gcda *.gcno
