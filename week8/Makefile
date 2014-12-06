APPS=test
APPS+=charcell-signal-kip
CFLAGS+= -g -D_GNU_SOURCE -std=c99
LIBS+= -L. -l attachable_debugger

include Makefile.so

all:: ${APPS} 


charcell-signal-kip: LIBS=-lncurses
charcell-signal-kip: charcell-utils.o

# both object files depend on the header file


clean::
	${RM} ${APPS} *.o *~ *.bak

# a static-pattern rule for all APPS
${APPS}: % : %.o
	${CC} ${LDFLAGS} -o $@ $^ ${LIBS}

# % is just wildcard char, it will match all
# part matched by just % is called the stem
# $@ is the target you are building
# $^ are all the dependinces of the applicaton(right hand side of colon)

.PHONY: all clean

