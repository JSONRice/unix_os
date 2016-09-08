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
# 414/homework4/bin directory as 'install_data' This makefile contains
# no dependencies and is used during the clean target.
#########################################################################
ROOT = ../../..
LIBS = $(ROOT)/lib
LOG_LIB_NAME = log_mgr
MEM_LIB_NAME = shared_mem
BIN = $(ROOT)/bin
INCLUDE = $(ROOT)/include

PROGRAM = install_data
# The program source will have main in it.
PROGRAM_SRC = $(PROGRAM).cc
OBJS = $(PROGRAM_SRC:.cc=.o)
# CFLAGS combines all the libraries. These should have been previously built.
CFLAGS = -I$(INCLUDE) -L$(LIBS) -l$(LOG_LIB_NAME) -l$(MEM_LIB_NAME) -Wall
CC   = g++

# Suffix rule for autocompiling sources with local include.
.cc.o:
	$(CC) $(CFLAGS) -c $<

$(PROGRAM): $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) -o $(PROGRAM)

###
# Note that a 'make install' against the libraries within
# ./414/homework3/src/lib/ for 'make it' to work here.
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
