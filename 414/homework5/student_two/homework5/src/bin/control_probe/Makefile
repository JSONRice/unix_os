##########################################################
# Author : Student 2
# Desc   : This make file creates control_probe executable
# Date   : 
##########################################################

# initial setup
PROJDIR = ../../..
LIBDIR  = ${PROJDIR}/lib
INCDIR  = ${PROJDIR}/include
BINDIR  = ${PROJDIR}/bin

SRC     = control_probe.c
OBJECTS = $(SRC:.c=.o)
PROGRAM = control_probe
FLAGS   = -I${INCDIR}
LIBS    = ${LIBDIR}/liblog_mgr.a ${LIBDIR}/libshmlib.a ${LIBDIR}/libsocketlib.a
CC      = cc ${FLAGS}

# creating the object files
.c.o:
	${CC} -c $<

# creating the various targets
it: ${PROGRAM}

install: ${BINDIR}/${PROGRAM}
${BINDIR}/${PROGRAM}: ${PROGRAM}
	cp ${PROGRAM} ${BINDIR}

${PROGRAM}: ${OBJECTS}
	${CC} ${OBJECTS} ${LIBS} -lsocket -lnsl -o ${PROGRAM} 
	chmod 755 ${PROGRAM}
clean:
	rm -f ${OBJECTS} ${PROGRAM}

bare: clean
	rm -f ${BINDIR}/${PROGRAM}

depend: ${SRC}
	${CC} -xM ${SRC} > dependList
	sed -e '1,/^# DO NOT DELETE/!d' Makefile > make.tmp
	cat dependList >> make.tmp
	mv make.tmp Makefile
	rm dependList

# DO NOT DELETE THIS LINE
control_probe.o: control_probe.c
control_probe.o: /usr/include/sys/types.h
control_probe.o: /usr/include/sys/feature_tests.h
control_probe.o: /usr/include/sys/ccompile.h
control_probe.o: /usr/include/sys/isa_defs.h
control_probe.o: /usr/include/sys/isa_defs.h
control_probe.o: /usr/include/sys/machtypes.h
control_probe.o: /usr/include/sys/feature_tests.h
control_probe.o: /usr/include/sys/int_types.h
control_probe.o: /usr/include/sys/feature_tests.h
control_probe.o: /usr/include/sys/select.h
control_probe.o: /usr/include/sys/feature_tests.h
control_probe.o: /usr/include/sys/time_impl.h
control_probe.o: /usr/include/sys/feature_tests.h
control_probe.o: /usr/include/sys/time.h
control_probe.o: /usr/include/sys/feature_tests.h
control_probe.o: /usr/include/sys/types.h
control_probe.o: /usr/include/time.h
control_probe.o: /usr/include/sys/feature_tests.h
control_probe.o: /usr/include/iso/time_iso.h
control_probe.o: /usr/include/sys/feature_tests.h
control_probe.o: /usr/include/sys/types.h
control_probe.o: /usr/include/sys/time_impl.h
control_probe.o: /usr/include/sys/select.h
control_probe.o: /usr/include/fcntl.h
control_probe.o: /usr/include/sys/feature_tests.h
control_probe.o: /usr/include/sys/types.h
control_probe.o: /usr/include/sys/fcntl.h
control_probe.o: /usr/include/sys/feature_tests.h
control_probe.o: /usr/include/sys/types.h
control_probe.o: /usr/include/unistd.h
control_probe.o: /usr/include/sys/feature_tests.h
control_probe.o: /usr/include/sys/types.h
control_probe.o: /usr/include/sys/unistd.h
control_probe.o: /usr/include/sys/feature_tests.h
control_probe.o: /usr/include/stdio.h
control_probe.o: /usr/include/sys/feature_tests.h
control_probe.o: /usr/include/iso/stdio_iso.h
control_probe.o: /usr/include/sys/feature_tests.h
control_probe.o: /usr/include/sys/va_list.h
control_probe.o: /usr/include/sys/isa_defs.h
control_probe.o: /usr/include/stdio_tag.h
control_probe.o: /usr/include/stdio_impl.h
control_probe.o: /usr/include/sys/isa_defs.h
control_probe.o: /usr/include/iso/stdio_c99.h
control_probe.o: /usr/include/signal.h
control_probe.o: /usr/include/sys/feature_tests.h
control_probe.o: /usr/include/sys/types.h
control_probe.o: /usr/include/iso/signal_iso.h
control_probe.o: /usr/include/sys/iso/signal_iso.h
control_probe.o: /usr/include/sys/unistd.h
control_probe.o: /usr/include/sys/signal.h
control_probe.o: /usr/include/sys/feature_tests.h
control_probe.o: /usr/include/sys/iso/signal_iso.h
control_probe.o: /usr/include/sys/siginfo.h
control_probe.o: /usr/include/sys/feature_tests.h
control_probe.o: /usr/include/sys/types.h
control_probe.o: /usr/include/sys/machsig.h
control_probe.o: /usr/include/sys/feature_tests.h
control_probe.o: /usr/include/sys/time_impl.h
control_probe.o: /usr/include/sys/types.h
control_probe.o: /usr/include/sys/procset.h
control_probe.o: /usr/include/sys/feature_tests.h
control_probe.o: /usr/include/sys/types.h
control_probe.o: /usr/include/sys/signal.h
control_probe.o: /usr/include/sys/siginfo.h
control_probe.o: /usr/include/time.h
control_probe.o: ../../../include/control_probe.h
control_probe.o: ../../../include/messages.h
control_probe.o: ../../../include/socket_utils.h
control_probe.o: /usr/include/errno.h
control_probe.o: /usr/include/sys/errno.h
control_probe.o: /usr/include/stdio.h
control_probe.o: /usr/include/sys/file.h
control_probe.o: /usr/include/sys/t_lock.h
control_probe.o: /usr/include/sys/machlock.h
control_probe.o: /usr/include/sys/types.h
control_probe.o: /usr/include/sys/param.h
control_probe.o: /usr/include/sys/types.h
control_probe.o: /usr/include/sys/isa_defs.h
control_probe.o: /usr/include/sys/unistd.h
control_probe.o: /usr/include/sys/mutex.h
control_probe.o: /usr/include/sys/types.h
control_probe.o: /usr/include/sys/rwlock.h
control_probe.o: /usr/include/sys/types.h
control_probe.o: /usr/include/sys/semaphore.h
control_probe.o: /usr/include/sys/condvar.h
control_probe.o: /usr/include/sys/types.h
control_probe.o: /usr/include/sys/time.h
control_probe.o: /usr/include/sys/types.h
control_probe.o: /usr/include/sys/socket.h
control_probe.o: /usr/include/sys/types.h
control_probe.o: /usr/include/sys/uio.h
control_probe.o: /usr/include/sys/feature_tests.h
control_probe.o: /usr/include/sys/types.h
control_probe.o: /usr/include/sys/feature_tests.h
control_probe.o: /usr/include/sys/socket_impl.h
control_probe.o: /usr/include/sys/un.h
control_probe.o: /usr/include/net/if_dl.h
control_probe.o: /usr/include/sys/netconfig.h
control_probe.o: /usr/include/netinet/in.h
control_probe.o: /usr/include/sys/feature_tests.h
control_probe.o: /usr/include/sys/types.h
control_probe.o: /usr/include/sys/socket_impl.h
control_probe.o: /usr/include/sys/stream.h
control_probe.o: /usr/include/sys/isa_defs.h
control_probe.o: /usr/include/sys/vnode.h
control_probe.o: /usr/include/sys/types.h
control_probe.o: /usr/include/sys/t_lock.h
control_probe.o: /usr/include/sys/rwstlock.h
control_probe.o: /usr/include/sys/types.h
control_probe.o: /usr/include/sys/ksynch.h
control_probe.o: /usr/include/sys/t_lock.h
control_probe.o: /usr/include/sys/rwlock.h
control_probe.o: /usr/include/sys/time_impl.h
control_probe.o: /usr/include/sys/cred.h
control_probe.o: /usr/include/sys/types.h
control_probe.o: /usr/include/sys/uio.h
control_probe.o: /usr/include/sys/resource.h
control_probe.o: /usr/include/sys/feature_tests.h
control_probe.o: /usr/include/sys/types.h
control_probe.o: /usr/include/sys/time.h
control_probe.o: /usr/include/vm/seg_enum.h
control_probe.o: /usr/include/sys/kstat.h
control_probe.o: /usr/include/sys/types.h
control_probe.o: /usr/include/sys/time.h
control_probe.o: /usr/include/sys/kmem.h
control_probe.o: /usr/include/sys/types.h
control_probe.o: /usr/include/sys/vmem.h
control_probe.o: /usr/include/sys/types.h
control_probe.o: /usr/include/sys/poll.h
control_probe.o: /usr/include/sys/strmdep.h
control_probe.o: /usr/include/sys/cred.h
control_probe.o: /usr/include/sys/t_lock.h
control_probe.o: /usr/include/sys/model.h
control_probe.o: /usr/include/sys/isa_defs.h
control_probe.o: /usr/include/sys/strft.h
control_probe.o: /usr/include/sys/time.h
control_probe.o: /usr/include/sys/byteorder.h
control_probe.o: /usr/include/sys/isa_defs.h
control_probe.o: /usr/include/netinet/in.h
control_probe.o: /usr/include/netdb.h
control_probe.o: /usr/include/sys/types.h
control_probe.o: /usr/include/netinet/in.h
control_probe.o: /usr/include/sys/socket.h
control_probe.o: /usr/include/sys/feature_tests.h
control_probe.o: ../../../include/shared_mem.h
control_probe.o: ../../../include/log_mgr.h
