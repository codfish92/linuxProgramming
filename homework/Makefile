APPS=crr 
LIBS= -lcurses
LIBS+= -L. -lattachable_debugger
LDFLAGS += -Wl,--no-as-needed
CFLAGS+= -g -D_GNU_SOURCE -std=c99

include Makefile.so

all::  ${APPS}

${APPS}: % : %.c


clean:: 
	${RM} ${APPS}  *.o *~ *.bak


crr: crr.o
	${CC} ${LDFLAGS} -o crr crr.o ${LIBS}

.PHONY: all clean
