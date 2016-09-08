########################################################
# Author : Student 2
# Desc   : This make file creates the libarg.a libary
# Date   : 
########################################################

# initial setup 
PROJDIR = ../../..
LIBDIR  = ${PROJDIR}/lib
INCDIR  = ${PROJDIR}/include
BINDIR  = ${PROJDIR}/bin

SRC     = log_mgr.c
OBJECTS = $(SRC:.c=.o)
PROGRAM = liblog_mgr.a
FLAGS   = -I${INCDIR}
CC      = cc ${FLAGS}

# creating teh object files
.c.o:
	${CC} -c $<

# making the varios targets
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
log_mgr.o: log_mgr.c
log_mgr.o: ../../../include/log_mgr.h
log_mgr.o: /usr/include/stdio.h
log_mgr.o: /usr/include/sys/feature_tests.h
log_mgr.o: /usr/include/sys/ccompile.h
log_mgr.o: /usr/include/sys/isa_defs.h
log_mgr.o: /usr/include/iso/stdio_iso.h
log_mgr.o: /usr/include/sys/feature_tests.h
log_mgr.o: /usr/include/sys/va_list.h
log_mgr.o: /usr/include/sys/isa_defs.h
log_mgr.o: /usr/include/stdio_tag.h
log_mgr.o: /usr/include/stdio_impl.h
log_mgr.o: /usr/include/sys/isa_defs.h
log_mgr.o: /usr/include/iso/stdio_c99.h
log_mgr.o: /usr/include/stdarg.h
log_mgr.o: /usr/include/iso/stdarg_iso.h
log_mgr.o: /usr/include/sys/va_impl.h
log_mgr.o: /usr/include/sys/va_list.h
log_mgr.o: /usr/include/iso/stdarg_c99.h
log_mgr.o: /usr/include/sys/feature_tests.h
log_mgr.o: /usr/include/time.h
log_mgr.o: /usr/include/sys/feature_tests.h
log_mgr.o: /usr/include/iso/time_iso.h
log_mgr.o: /usr/include/sys/feature_tests.h
log_mgr.o: /usr/include/sys/types.h
log_mgr.o: /usr/include/sys/feature_tests.h
log_mgr.o: /usr/include/sys/isa_defs.h
log_mgr.o: /usr/include/sys/machtypes.h
log_mgr.o: /usr/include/sys/feature_tests.h
log_mgr.o: /usr/include/sys/int_types.h
log_mgr.o: /usr/include/sys/feature_tests.h
log_mgr.o: /usr/include/sys/select.h
log_mgr.o: /usr/include/sys/feature_tests.h
log_mgr.o: /usr/include/sys/time_impl.h
log_mgr.o: /usr/include/sys/feature_tests.h
log_mgr.o: /usr/include/sys/time.h
log_mgr.o: /usr/include/sys/feature_tests.h
log_mgr.o: /usr/include/sys/types.h
log_mgr.o: /usr/include/time.h
log_mgr.o: /usr/include/sys/select.h
log_mgr.o: /usr/include/sys/time_impl.h
log_mgr.o: /usr/include/string.h
log_mgr.o: /usr/include/iso/string_iso.h
log_mgr.o: /usr/include/sys/feature_tests.h
log_mgr.o: /usr/include/sys/file.h
log_mgr.o: /usr/include/sys/t_lock.h
log_mgr.o: /usr/include/sys/machlock.h
log_mgr.o: /usr/include/sys/types.h
log_mgr.o: /usr/include/sys/param.h
log_mgr.o: /usr/include/sys/types.h
log_mgr.o: /usr/include/sys/isa_defs.h
log_mgr.o: /usr/include/sys/unistd.h
log_mgr.o: /usr/include/sys/feature_tests.h
log_mgr.o: /usr/include/sys/mutex.h
log_mgr.o: /usr/include/sys/types.h
log_mgr.o: /usr/include/sys/rwlock.h
log_mgr.o: /usr/include/sys/types.h
log_mgr.o: /usr/include/sys/semaphore.h
log_mgr.o: /usr/include/sys/condvar.h
log_mgr.o: /usr/include/sys/types.h
log_mgr.o: /usr/include/sys/time.h
log_mgr.o: /usr/include/sys/fcntl.h
log_mgr.o: /usr/include/sys/feature_tests.h
log_mgr.o: /usr/include/sys/types.h
log_mgr.o: /usr/include/unistd.h
log_mgr.o: /usr/include/sys/feature_tests.h
log_mgr.o: /usr/include/sys/types.h
log_mgr.o: /usr/include/sys/unistd.h
