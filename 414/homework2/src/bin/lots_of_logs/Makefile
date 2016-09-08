#########################################################################
# Student: Jason Rice
#
# Course: 605.414 Systems Development in a Unix Environment
#
# File: Makefile
#
# Description:
#   This is a node (leaf) level Makefile and acts upon the contents
# encapsulated within the current working directory. This Makefile is
# responsible for compiling and linking in necessary libraries in order
# to generate a lots_of_logs executable that is then placed within the
# 414/homework2/bin directory as 'lots_of_logs' This makefile contains
# no dependencies and is used during the clean target.
#########################################################################
ROOT = ../../..
LIBS = $(ROOT)/lib
LIB_NAME = log_mgr
BIN = $(ROOT)/bin
INCLUDE = $(ROOT)/include

PROGRAM = lots_of_logs
# The program source will have main in it.
PROGRAM_SRC = $(PROGRAM).c
OBJS = $(PROGRAM_SRC:.c=.o)
# CFLAGS combines all the libraries. These should have been previously built.
CFLAGS = -I$(INCLUDE) -L$(LIBS) -l$(LIB_NAME)
CC   = gcc

# Suffix rule for autocompiling sources with local include.
.c.o:
	$(CC) $(CFLAGS) -c $<

$(PROGRAM): $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) -o $(PROGRAM)

###
# Note that a 'make install' against the libraries within
# ./414/homework2/src/lib/ for 'make it' to work here.
###
it: $(PROGRAM)
	@echo "Compiling sources with libraries."

install: $(PROGRAM)
	cp $(PROGRAM) $(BIN)
	@echo "Executable named '$(PROGRAM)' was placed in '$(BIN)' and is ready to run."

clean: 
	@echo "Cleaning up object files and executables. Clearing dependencies."
	rm -f $(OBJS) $(BIN)/$(PROGRAM) $(PROGRAM)
	cp Makefile.nodependencies Makefile

depend: $(PROGRAM_SRC)
	@echo "Generating the dependencies."
	$(CC) -M $(CFLAGS) $(PROGRAM_SRC) > dependencies
	sed -e '1,/^# DO NOT DELETE/!d' Makefile > make.tmp
	cat dependencies >> make.tmp
	mv make.tmp Makefile
	rm dependencies

# DO NOT DELETE THIS LINE - make depend uses this
lots_of_logs.o: lots_of_logs.c /usr/include/unistd.h \
 /usr/include/sys/feature_tests.h /usr/include/sys/ccompile.h \
 /usr/include/sys/isa_defs.h /usr/include/sys/types.h \
 /usr/include/sys/machtypes.h /usr/include/sys/int_types.h \
 /usr/include/sys/select.h /usr/include/sys/time_impl.h \
 /usr/include/sys/time.h /usr/include/time.h /usr/include/iso/time_iso.h \
 /usr/include/sys/unistd.h /usr/include/stdlib.h \
 /usr/include/iso/stdlib_iso.h /usr/include/iso/stdlib_c99.h \
 ../../../include/log_mgr.h
