# Makefile for liblog.a 
# $Header: Makefile$
#
# Project:	
# Module:	
# Programmer:	John C. Noble
# Organization:	EP
# Host System:	SunOS 4.0.1
# Date Created: 03-01-93
# Modifications:
#	$Log$
# 
# 

# Library to make (eg: libeg.a)
LIBRARY = libsock.a 
# Program for testing LIBRARY (eg: egtest)
# Directory name where this code resides
PROJECT_ROOT	= ../../..
BIN		= $(PROJECT_ROOT)/bin
LIB		= $(PROJECT_ROOT)/lib
INCLUDE		= $(PROJECT_ROOT)/include
PROGRAM = try

DEBUG	= DEBUG
VERSION	= BSD
CFLAGS	= -g -D$(DEBUG) -D$(VERSION) -I$(INCLUDE)
CC	= gcc $(CFLAGS)
AR	= ar cr

README	= Readme
MAKEFILE = Makefile

# Source files for LIBRARY routines (don't include the test PROGRAM.c)
SRC	= setup_server.c \
		setup_client.c

# Object modules for LIBRARY (don't include the test PROGRAM.o)
OBJ	= setup_server.o \
		setup_client.o

# Other LIBS needed to compile the test PROGRAM (eg: /usr/local/lib/libother.a)
LIBS	= 
# How they are invoked on the compile line (eg: -lspecial)
LLIBS	= 

SRCS	= $(SRC)

.c.o:
	$(CC) -c $<

it: $(LIBRARY) 

all: local_side1 local_side2 remote_side1 remote_side2 tags

#
# some test programs
#
test_server: test_server.o $(LIBRARY)
	$(CC) $(CFLAGS) test_server.o $(LIBRARY) -o test_server -lsocket -lnsl

test_client: test_client.o $(LIBRARY)
	$(CC) $(CFLAGS) test_client.o $(LIBRARY) -o test_client -lsocket -lnsl

local_side1: $(LIBRARY) local_side1.o
	$(CC) $(CFLAGS) local_side1.o $(LIBRARY) -o local_side1

local_side2: $(LIBRARY) local_side2.o
	$(CC) $(CFLAGS) local_side2.o $(LIBRARY) -o local_side2

remote_side1: $(LIBRARY) remote_side1.o
	$(CC) $(CFLAGS) remote_side1.o $(LIBRARY) -o remote_side1

remote_side2: $(LIBRARY) remote_side2.o
	$(CC) $(CFLAGS) remote_side2.o $(LIBRARY) -o remote_side2

install: $(LIB)/$(LIBRARY)
$(LIB)/$(LIBRARY): $(LIBRARY)
	cp $(LIBRARY) $(LIB)
	ranlib $(LIB)/$(LIBRARY)

$(PROGRAM): $(OBJ) $(LIBS) $(LIBRARY) $(PROGRAM).o
	$(CC) $(PROGRAM).o $(LIBRARY) -o $(PROGRAM)

$(LIBRARY): $(OBJ)
	$(AR) $(LIBRARY) $(OBJ)
	ranlib $(LIBRARY)

$(OBJ): $(HDR) $(MAKEFILE)

$(SRCS):
	co $@

tags: $(HDR) $(SRCS)
	ctags $(HDR) $(SRCS)

clean cleanup:
	rm -f $(PROGRAM).o $(OBJ) core a.out mon.out gmon.out 

bare: clean
	rm -f $(LISTING) $(PROFILE) $(XREF)

depend: $(SRCS)
	rm -f dependlist;
	for i in $(SRC); do $(CC) -xM -I$(INCLUDE) $$i >> dependlist; done
	sed -e '1,/^# DO NOT DELETE/!d' Makefile > /tmp/mm.tmp
	cat dependlist >> /tmp/mm.tmp
	mv /tmp/mm.tmp Makefile
	rm -f dependlist

# DO NOT DELETE THIS LINE - make depend uses it
setup_server.o: setup_server.c
setup_server.o: /usr/include/unistd.h
setup_server.o: /usr/include/sys/feature_tests.h
setup_server.o: /usr/include/sys/ccompile.h
setup_server.o: /usr/include/sys/isa_defs.h
setup_server.o: /usr/include/sys/types.h
setup_server.o: /usr/include/sys/feature_tests.h
setup_server.o: /usr/include/sys/isa_defs.h
setup_server.o: /usr/include/sys/machtypes.h
setup_server.o: /usr/include/sys/feature_tests.h
setup_server.o: /usr/include/sys/int_types.h
setup_server.o: /usr/include/sys/feature_tests.h
setup_server.o: /usr/include/sys/select.h
setup_server.o: /usr/include/sys/feature_tests.h
setup_server.o: /usr/include/sys/time_impl.h
setup_server.o: /usr/include/sys/feature_tests.h
setup_server.o: /usr/include/sys/time.h
setup_server.o: /usr/include/sys/feature_tests.h
setup_server.o: /usr/include/sys/types.h
setup_server.o: /usr/include/time.h
setup_server.o: /usr/include/sys/feature_tests.h
setup_server.o: /usr/include/iso/time_iso.h
setup_server.o: /usr/include/sys/feature_tests.h
setup_server.o: /usr/include/sys/types.h
setup_server.o: /usr/include/sys/time_impl.h
setup_server.o: /usr/include/sys/select.h
setup_server.o: /usr/include/sys/unistd.h
setup_server.o: /usr/include/sys/feature_tests.h
setup_server.o: /usr/include/strings.h
setup_server.o: /usr/include/sys/types.h
setup_server.o: /usr/include/sys/feature_tests.h
setup_server.o: /usr/include/string.h
setup_server.o: /usr/include/iso/string_iso.h
setup_server.o: /usr/include/sys/feature_tests.h
setup_server.o: ../../../include/socket_utils.h
setup_server.o: /usr/include/errno.h
setup_server.o: /usr/include/sys/errno.h
setup_server.o: /usr/include/stdio.h
setup_server.o: /usr/include/sys/feature_tests.h
setup_server.o: /usr/include/iso/stdio_iso.h
setup_server.o: /usr/include/sys/feature_tests.h
setup_server.o: /usr/include/sys/va_list.h
setup_server.o: /usr/include/sys/isa_defs.h
setup_server.o: /usr/include/stdio_tag.h
setup_server.o: /usr/include/stdio_impl.h
setup_server.o: /usr/include/sys/isa_defs.h
setup_server.o: /usr/include/iso/stdio_c99.h
setup_server.o: /usr/include/sys/file.h
setup_server.o: /usr/include/sys/t_lock.h
setup_server.o: /usr/include/sys/machlock.h
setup_server.o: /usr/include/sys/types.h
setup_server.o: /usr/include/sys/param.h
setup_server.o: /usr/include/sys/types.h
setup_server.o: /usr/include/sys/isa_defs.h
setup_server.o: /usr/include/sys/unistd.h
setup_server.o: /usr/include/sys/mutex.h
setup_server.o: /usr/include/sys/types.h
setup_server.o: /usr/include/sys/rwlock.h
setup_server.o: /usr/include/sys/types.h
setup_server.o: /usr/include/sys/semaphore.h
setup_server.o: /usr/include/sys/condvar.h
setup_server.o: /usr/include/sys/types.h
setup_server.o: /usr/include/sys/time.h
setup_server.o: /usr/include/sys/types.h
setup_server.o: /usr/include/sys/socket.h
setup_server.o: /usr/include/sys/types.h
setup_server.o: /usr/include/sys/uio.h
setup_server.o: /usr/include/sys/feature_tests.h
setup_server.o: /usr/include/sys/types.h
setup_server.o: /usr/include/sys/feature_tests.h
setup_server.o: /usr/include/sys/socket_impl.h
setup_server.o: /usr/include/sys/un.h
setup_server.o: /usr/include/net/if_dl.h
setup_server.o: /usr/include/sys/netconfig.h
setup_server.o: /usr/include/netinet/in.h
setup_server.o: /usr/include/sys/feature_tests.h
setup_server.o: /usr/include/sys/types.h
setup_server.o: /usr/include/sys/socket_impl.h
setup_server.o: /usr/include/sys/stream.h
setup_server.o: /usr/include/sys/isa_defs.h
setup_server.o: /usr/include/sys/vnode.h
setup_server.o: /usr/include/sys/types.h
setup_server.o: /usr/include/sys/t_lock.h
setup_server.o: /usr/include/sys/rwstlock.h
setup_server.o: /usr/include/sys/types.h
setup_server.o: /usr/include/sys/ksynch.h
setup_server.o: /usr/include/sys/t_lock.h
setup_server.o: /usr/include/sys/rwlock.h
setup_server.o: /usr/include/sys/time_impl.h
setup_server.o: /usr/include/sys/cred.h
setup_server.o: /usr/include/sys/types.h
setup_server.o: /usr/include/sys/uio.h
setup_server.o: /usr/include/sys/resource.h
setup_server.o: /usr/include/sys/feature_tests.h
setup_server.o: /usr/include/sys/types.h
setup_server.o: /usr/include/sys/time.h
setup_server.o: /usr/include/vm/seg_enum.h
setup_server.o: /usr/include/sys/kstat.h
setup_server.o: /usr/include/sys/types.h
setup_server.o: /usr/include/sys/time.h
setup_server.o: /usr/include/sys/kmem.h
setup_server.o: /usr/include/sys/types.h
setup_server.o: /usr/include/sys/vmem.h
setup_server.o: /usr/include/sys/types.h
setup_server.o: /usr/include/sys/poll.h
setup_server.o: /usr/include/sys/strmdep.h
setup_server.o: /usr/include/sys/cred.h
setup_server.o: /usr/include/sys/t_lock.h
setup_server.o: /usr/include/sys/model.h
setup_server.o: /usr/include/sys/isa_defs.h
setup_server.o: /usr/include/sys/strft.h
setup_server.o: /usr/include/sys/time.h
setup_server.o: /usr/include/sys/byteorder.h
setup_server.o: /usr/include/sys/isa_defs.h
setup_server.o: /usr/include/netinet/in.h
setup_server.o: /usr/include/netdb.h
setup_server.o: /usr/include/sys/types.h
setup_server.o: /usr/include/netinet/in.h
setup_server.o: /usr/include/sys/socket.h
setup_server.o: /usr/include/sys/feature_tests.h
setup_client.o: setup_client.c
setup_client.o: /usr/include/unistd.h
setup_client.o: /usr/include/sys/feature_tests.h
setup_client.o: /usr/include/sys/ccompile.h
setup_client.o: /usr/include/sys/isa_defs.h
setup_client.o: /usr/include/sys/types.h
setup_client.o: /usr/include/sys/feature_tests.h
setup_client.o: /usr/include/sys/isa_defs.h
setup_client.o: /usr/include/sys/machtypes.h
setup_client.o: /usr/include/sys/feature_tests.h
setup_client.o: /usr/include/sys/int_types.h
setup_client.o: /usr/include/sys/feature_tests.h
setup_client.o: /usr/include/sys/select.h
setup_client.o: /usr/include/sys/feature_tests.h
setup_client.o: /usr/include/sys/time_impl.h
setup_client.o: /usr/include/sys/feature_tests.h
setup_client.o: /usr/include/sys/time.h
setup_client.o: /usr/include/sys/feature_tests.h
setup_client.o: /usr/include/sys/types.h
setup_client.o: /usr/include/time.h
setup_client.o: /usr/include/sys/feature_tests.h
setup_client.o: /usr/include/iso/time_iso.h
setup_client.o: /usr/include/sys/feature_tests.h
setup_client.o: /usr/include/sys/types.h
setup_client.o: /usr/include/sys/time_impl.h
setup_client.o: /usr/include/sys/select.h
setup_client.o: /usr/include/sys/unistd.h
setup_client.o: /usr/include/sys/feature_tests.h
setup_client.o: /usr/include/strings.h
setup_client.o: /usr/include/sys/types.h
setup_client.o: /usr/include/sys/feature_tests.h
setup_client.o: /usr/include/string.h
setup_client.o: /usr/include/iso/string_iso.h
setup_client.o: /usr/include/sys/feature_tests.h
setup_client.o: ../../../include/socket_utils.h
setup_client.o: /usr/include/errno.h
setup_client.o: /usr/include/sys/errno.h
setup_client.o: /usr/include/stdio.h
setup_client.o: /usr/include/sys/feature_tests.h
setup_client.o: /usr/include/iso/stdio_iso.h
setup_client.o: /usr/include/sys/feature_tests.h
setup_client.o: /usr/include/sys/va_list.h
setup_client.o: /usr/include/sys/isa_defs.h
setup_client.o: /usr/include/stdio_tag.h
setup_client.o: /usr/include/stdio_impl.h
setup_client.o: /usr/include/sys/isa_defs.h
setup_client.o: /usr/include/iso/stdio_c99.h
setup_client.o: /usr/include/sys/file.h
setup_client.o: /usr/include/sys/t_lock.h
setup_client.o: /usr/include/sys/machlock.h
setup_client.o: /usr/include/sys/types.h
setup_client.o: /usr/include/sys/param.h
setup_client.o: /usr/include/sys/types.h
setup_client.o: /usr/include/sys/isa_defs.h
setup_client.o: /usr/include/sys/unistd.h
setup_client.o: /usr/include/sys/mutex.h
setup_client.o: /usr/include/sys/types.h
setup_client.o: /usr/include/sys/rwlock.h
setup_client.o: /usr/include/sys/types.h
setup_client.o: /usr/include/sys/semaphore.h
setup_client.o: /usr/include/sys/condvar.h
setup_client.o: /usr/include/sys/types.h
setup_client.o: /usr/include/sys/time.h
setup_client.o: /usr/include/sys/types.h
setup_client.o: /usr/include/sys/socket.h
setup_client.o: /usr/include/sys/types.h
setup_client.o: /usr/include/sys/uio.h
setup_client.o: /usr/include/sys/feature_tests.h
setup_client.o: /usr/include/sys/types.h
setup_client.o: /usr/include/sys/feature_tests.h
setup_client.o: /usr/include/sys/socket_impl.h
setup_client.o: /usr/include/sys/un.h
setup_client.o: /usr/include/net/if_dl.h
setup_client.o: /usr/include/sys/netconfig.h
setup_client.o: /usr/include/netinet/in.h
setup_client.o: /usr/include/sys/feature_tests.h
setup_client.o: /usr/include/sys/types.h
setup_client.o: /usr/include/sys/socket_impl.h
setup_client.o: /usr/include/sys/stream.h
setup_client.o: /usr/include/sys/isa_defs.h
setup_client.o: /usr/include/sys/vnode.h
setup_client.o: /usr/include/sys/types.h
setup_client.o: /usr/include/sys/t_lock.h
setup_client.o: /usr/include/sys/rwstlock.h
setup_client.o: /usr/include/sys/types.h
setup_client.o: /usr/include/sys/ksynch.h
setup_client.o: /usr/include/sys/t_lock.h
setup_client.o: /usr/include/sys/rwlock.h
setup_client.o: /usr/include/sys/time_impl.h
setup_client.o: /usr/include/sys/cred.h
setup_client.o: /usr/include/sys/types.h
setup_client.o: /usr/include/sys/uio.h
setup_client.o: /usr/include/sys/resource.h
setup_client.o: /usr/include/sys/feature_tests.h
setup_client.o: /usr/include/sys/types.h
setup_client.o: /usr/include/sys/time.h
setup_client.o: /usr/include/vm/seg_enum.h
setup_client.o: /usr/include/sys/kstat.h
setup_client.o: /usr/include/sys/types.h
setup_client.o: /usr/include/sys/time.h
setup_client.o: /usr/include/sys/kmem.h
setup_client.o: /usr/include/sys/types.h
setup_client.o: /usr/include/sys/vmem.h
setup_client.o: /usr/include/sys/types.h
setup_client.o: /usr/include/sys/poll.h
setup_client.o: /usr/include/sys/strmdep.h
setup_client.o: /usr/include/sys/cred.h
setup_client.o: /usr/include/sys/t_lock.h
setup_client.o: /usr/include/sys/model.h
setup_client.o: /usr/include/sys/isa_defs.h
setup_client.o: /usr/include/sys/strft.h
setup_client.o: /usr/include/sys/time.h
setup_client.o: /usr/include/sys/byteorder.h
setup_client.o: /usr/include/sys/isa_defs.h
setup_client.o: /usr/include/netinet/in.h
setup_client.o: /usr/include/netdb.h
setup_client.o: /usr/include/sys/types.h
setup_client.o: /usr/include/netinet/in.h
setup_client.o: /usr/include/sys/socket.h
setup_client.o: /usr/include/sys/feature_tests.h
