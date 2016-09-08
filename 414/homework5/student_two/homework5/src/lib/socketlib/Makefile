########################################################
# Author : Student 2
# Desc   : This make file creates the shared_mem.a libary
# Date   : 
########################################################

# initial setup
PROJDIR = ../../..
LIBDIR  = ${PROJDIR}/lib
INCDIR  = ${PROJDIR}/include
BINDIR  = ${PROJDIR}/bin

SRC     = setup_client.c setup_server.c
OBJECTS = $(SRC:.c=.o)
PROGRAM = libsocketlib.a
FLAGS   = -I${INCDIR}
CC      = cc ${FLAGS}

# creating the object files
.c.o:
	${CC} -c $<

# making the various targets
it: ${PROGRAM}
	
install: ${LIBDIR}/${PROGRAM}
${LIBDIR}/${PROGRAM}: ${PROGRAM}
	cp ${PROGRAM} ${LIBDIR}

${PROGRAM}: ${OBJECTS}
	ar -r ${PROGRAM} ${OBJECTS} 

clean:
	rm -f ${OBJECTS} ${PROGRAM}

bare: clean
	rm -f ${LIBDIR}/${PROGRAM}

depend: ${SRC}
	${CC} -xM ${SRC} > dependList
	sed -e '1,/^# DO NOT DELETE/!d' Makefile > make.tmp
	cat dependList >> make.tmp
	mv make.tmp Makefile
	rm dependList

# DO NOT DELETE THIS LINE
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
