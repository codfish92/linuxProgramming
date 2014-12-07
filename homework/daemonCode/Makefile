APPS+=temp
LIBS+= -L. -lattachable_debugger
LDFLAGS += -Wl,--no-as-needed
CFLAGS+= -g -D_GNU_SOURCE -std=c99

include Makefile.so

all:: ${APPS} 

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

