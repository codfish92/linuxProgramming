#flags to throw at gcc
CFLAGS=-g
#rulename: what it depends on
#first rule, implicilty ran if no rule is provided
#build sampleC, depends on object code
sampleC: sampleC.o

#build object code, depends on source code
sampleC.o: sampleC.c


#tells make to ignore any file named clean in directory
.PHONY: clean
clean: 
	${RM} *~ *.bak sampleC sampleC.o

