CC =		gcc
CFLAGS  =	$(FLAGS) -c  -I$(INCLUDEPATH) 

OBJS=  SRTMrdf.o rdftest.o

$(shell mkdir -p $(MACHTYPE)-$(OSTYPE))

.KEEP_STATE:
all:	$(OBJS)
	mv *.o $(MACHTYPE)-$(OSTYPE)





