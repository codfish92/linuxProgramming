APPS= charcell-resize
APPS+= signal_example

CFLAGS+= -g -D_GNU_SOURCE -std=c99

all: ${APPS} 

# both object files depend on the header file
swc.o swcutils.o: swcutils.h

# the swc application also needs swcutils.o
swc: swcutils.o

# the charcell application needs the ncurses library, the other apps don't
charcell: LIBS=-lncurses
charcell-resize: LIBS=-lncurses

signal_example: signal_example.o signal_example_utils.o

clean: 
	${RM} ${APPS} *.o *~ *.bak

# a static-pattern rule for all APPS
${APPS}: % : %.o
	${CC} ${LDFLAGS} -o $@ $^ ${LIBS}

# % is just wildcard char, it will match all
# part matched by just % is called the stem
# $@ is the target you are building
# $^ are all the dependinces of the applicaton(right hand side of colon)

.PHONY: all clean
