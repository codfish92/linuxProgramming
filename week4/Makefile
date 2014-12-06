APPS=
CFLAGS+= -g -D_GNU_SOURCE -std=c99

all:  ${APPS} swc

${APPS}: % : %.c

swc: swc.o swcutil.o

swc.o swcutil.o: swcutil.h


clean: 
	${RM} ${APPS} swc *.o *~ *.bak


.PHONY: all clean
