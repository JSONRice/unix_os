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
SRCS     = log_mgr.c
OBJS     = $(SRCS:.c=.o)
CFLAGS   = -I$(INCLUDE)
CC       = gcc $(CFLAGS)
LIB_NAME = log_mgr
ARCHIVE  = lib$(LIB_NAME).a

# Suffix rule for autocompiling sources with local include.
.c.o:
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
log_mgr.o: log_mgr.c ../../../include/log_mgr.h \
 /usr/gcc/4.5/lib/gcc/sparc-sun-solaris2.11/4.5.2/include/stdarg.h \
 /usr/include/string.h /usr/include/iso/string_iso.h \
 /usr/include/sys/feature_tests.h /usr/include/sys/ccompile.h \
 /usr/include/sys/isa_defs.h /usr/include/stdlib.h \
 /usr/include/iso/stdlib_iso.h /usr/include/iso/stdlib_c99.h \
 /usr/include/fcntl.h /usr/include/sys/types.h \
 /usr/include/sys/machtypes.h /usr/include/sys/int_types.h \
 /usr/include/sys/select.h /usr/include/sys/time_impl.h \
 /usr/include/sys/time.h /usr/include/time.h /usr/include/iso/time_iso.h \
 /usr/include/sys/fcntl.h
