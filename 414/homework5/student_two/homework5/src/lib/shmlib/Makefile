########################################################
# Author : Student 2
# Desc   : This make file creates the shared_mem.a libary
# Date   : 
########################################################

#initial setup
PROJDIR = ../../..
LIBDIR  = ${PROJDIR}/lib
INCDIR  = ${PROJDIR}/include
BINDIR  = ${PROJDIR}/bin

SRC     = shared_mem.c
OBJECTS = $(SRC:.c=.o)
PROGRAM = libshmlib.a
FLAGS   = -I${INCDIR}
CC      = cc ${FLAGS}

# creating the object files
.c.o:
	${CC} -c $<

# creating the various targets
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
shared_mem.o: shared_mem.c
shared_mem.o: /usr/include/sys/types.h
shared_mem.o: /usr/include/sys/feature_tests.h
shared_mem.o: /usr/include/sys/ccompile.h
shared_mem.o: /usr/include/sys/isa_defs.h
shared_mem.o: /usr/include/sys/isa_defs.h
shared_mem.o: /usr/include/sys/machtypes.h
shared_mem.o: /usr/include/sys/feature_tests.h
shared_mem.o: /usr/include/sys/int_types.h
shared_mem.o: /usr/include/sys/feature_tests.h
shared_mem.o: /usr/include/sys/select.h
shared_mem.o: /usr/include/sys/feature_tests.h
shared_mem.o: /usr/include/sys/time_impl.h
shared_mem.o: /usr/include/sys/feature_tests.h
shared_mem.o: /usr/include/sys/time.h
shared_mem.o: /usr/include/sys/feature_tests.h
shared_mem.o: /usr/include/sys/types.h
shared_mem.o: /usr/include/time.h
shared_mem.o: /usr/include/sys/feature_tests.h
shared_mem.o: /usr/include/iso/time_iso.h
shared_mem.o: /usr/include/sys/feature_tests.h
shared_mem.o: /usr/include/sys/types.h
shared_mem.o: /usr/include/sys/time_impl.h
shared_mem.o: /usr/include/sys/select.h
shared_mem.o: /usr/include/sys/param.h
shared_mem.o: /usr/include/sys/types.h
shared_mem.o: /usr/include/sys/isa_defs.h
shared_mem.o: /usr/include/sys/unistd.h
shared_mem.o: /usr/include/sys/feature_tests.h
shared_mem.o: /usr/include/sys/ipc.h
shared_mem.o: /usr/include/sys/isa_defs.h
shared_mem.o: /usr/include/sys/feature_tests.h
shared_mem.o: /usr/include/sys/types.h
shared_mem.o: /usr/include/sys/shm.h
shared_mem.o: /usr/include/sys/ipc.h
shared_mem.o: /usr/include/sys/unistd.h
shared_mem.o: /usr/include/stdio.h
shared_mem.o: /usr/include/sys/feature_tests.h
shared_mem.o: /usr/include/iso/stdio_iso.h
shared_mem.o: /usr/include/sys/feature_tests.h
shared_mem.o: /usr/include/sys/va_list.h
shared_mem.o: /usr/include/sys/isa_defs.h
shared_mem.o: /usr/include/stdio_tag.h
shared_mem.o: /usr/include/stdio_impl.h
shared_mem.o: /usr/include/sys/isa_defs.h
shared_mem.o: /usr/include/iso/stdio_c99.h
shared_mem.o: ../../../include/shared_mem.h
