SHELL =		/bin/sh
CC =		gcc
ROOTDIR =	/Users/ian
PROGDIR =       $(ROOTDIR)/progs
INCLUDEPATH =	'-I$(ROOTDIR)/progs -I/usr/include '
BINDIR =	$(ROOTDIR)/bin
CFLAGS =	 '-O3 -m32'
CFLAGSL =	 '-O3 -m32' 
STANDARD =	$(PROGDIR)/clib/standard.o


RDFDIRS =	rdfRoutines 

rdf:
	@for i in ${RDFDIRS}; do \
		( 	echo "<<< Descending in directory: $$i >>>"; \
	                cd $$i; \
		       make INCLUDEPATH=$(INCLUDEPATH) FLAGS=$(CFLAGS);  \
			cd $(PROGDIR)/srtm; \
		); done
