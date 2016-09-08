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
# responsible for compiling and archiving the log manager library for 
# providing a convenient logging API for a programmer to use.
#########################################################################

ROOT     = ../../..
LIBS     = $(ROOT)/lib
INCLUDE  = $(ROOT)/include
SRCS     = log_mgr.cc
OBJS     = $(SRCS:.cc=.o)
CFLAGS   = -I$(INCLUDE)
CC       = g++ $(CFLAGS)
LIB_NAME = log_mgr
ARCHIVE  = lib$(LIB_NAME).a

# Suffix rule for autocompiling sources with local include.
.cc.o:
	$(CC) -c $<

$(ARCHIVE): $(OBJS)
	ar -cr $(ARCHIVE) $(OBJS)

###
# 'make it' just compiles. Use 'make install' to compile and copy to the library directory.
###
it: $(ARCHIVE)

# Package object code files into the library directory as an archive file: 
install: $(ARCHIVE)
	cp $(ARCHIVE) $(LIBS)
	@echo "Sources archived here $(LIBS)/$(ARCHIVE) Library ready for use."

clean: 
	@echo "Cleaning up object and library files. Removing dependencies."
	rm -f $(OBJS) $(ARCHIVE) $(LIBS)/$(ARCHIVE)
	cp Makefile.nodependencies Makefile

depend: $(SRCS)
	@echo "Generating the dependencies."
	$(CC) -M $(SRCS) > dependencies
	sed -e '1,/^# DO NOT DELETE/!d' Makefile > make.tmp
	cat dependencies >> make.tmp
	mv make.tmp Makefile
	rm dependencies

# DO NOT DELETE THIS LINE - make depend uses this
