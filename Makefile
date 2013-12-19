# -*- makefile -*-
TARGET:=u-search
include config.mk

libcppsockets:
	+cd $(SRCDIR)/cppsockets && $(MAKE)

spider: copyfiles libdata_storage
	+cd $(SRCDIR)/spider && $(MAKE)

scheduler:
	+cd $(SRCDIR)/scheduler && $(MAKE)

libdata_storage:
	+cd $(SRCDIR)/data-storage && $(MAKE)

test: libcppsockets libdata_storage spider scheduler
	+cd $(SRCDIR)/test && $(MAKE)

copyfiles: database.dat.example servers.dat.example
	mkdir -p $(DESTDIR)/etc/u-search
	cp database.dat.example $(DESTDIR)/etc/u-search
	cp servers.dat.example $(DESTDIR)/etc/u-search

$(TARGET): test doc

doc:
	cd $(SRCDIR)/doc && $(MAKE)

help:
	@echo Available modules: libcppsockets spider spider libdata_storage test copygiles doc
	@echo Debug mode: DEBUG=yes
	@echo Test coverage: TEST_COVERAGE=yes
	@echo Show build commands: VERBOSE=yes

clean:
	rm -rf build
	cd $(SRCDIR)/spider && make clean
	cd $(SRCDIR)/scheduler && make clean
	cd $(SRCDIR)/cppsockets && make clean
	cd $(SRCDIR)/data-storage && make clean
	cd $(SRCDIR)/test && make clean
	cd $(SRCDIR)/doc && make clean

.PHONY: help doc spider scheduler copyfiles libdata_storage libcppsockets test
