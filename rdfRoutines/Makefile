CC =		gcc
CFLAGS  =	$(FLAGS) -c  -I$(INCLUDEPATH) 

OBJS=  SRTMrdf.o rdftest.o

MACHTYPE = $(shell uname -m)
OSTYPE = $(shell uname -s)

$(shell mkdir -p $(MACHTYPE)-$(OSTYPE))

.KEEP_STATE:
all:	$(OBJS)
	mv *.o $(MACHTYPE)-$(OSTYPE)





