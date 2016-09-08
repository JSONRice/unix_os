##########################################################
# Author : Student 2
# Desc   : This make file creates display executable
# Date   : 
##########################################################

# setting up initial directies
PROJDIR = ../../..
LIBDIR  = ${PROJDIR}/lib
INCDIR  = ${PROJDIR}/include
BINDIR  = ${PROJDIR}/bin

# setting up flags and libraries the program will
# need to compile
SRC     = display.c
OBJECTS = $(SRC:.c=.o)
PROGRAM = display
FLAGS   = -I/usr/include 
CC      = cc -lcurses ${FLAGS}

# creating the object files
.c.o:
	${CC} ${FLAGS} -c $<

# making the various targets
it: ${PROGRAM}

install: ${BINDIR}/${PROGRAM}
${BINDIR}/${PROGRAM}: ${PROGRAM}
	cp ${PROGRAM} ${BINDIR}

${PROGRAM}: ${OBJECTS}
	${CC} ${OBJECTS} -o ${PROGRAM} 
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
display.o: display.c
display.o: /usr/include/stdio.h
display.o: /usr/include/sys/feature_tests.h
display.o: /usr/include/sys/ccompile.h
display.o: /usr/include/sys/isa_defs.h
display.o: /usr/include/iso/stdio_iso.h
display.o: /usr/include/sys/feature_tests.h
display.o: /usr/include/sys/va_list.h
display.o: /usr/include/sys/isa_defs.h
display.o: /usr/include/stdio_tag.h
display.o: /usr/include/stdio_impl.h
display.o: /usr/include/sys/isa_defs.h
display.o: /usr/include/iso/stdio_c99.h
display.o: /usr/include/curses.h
display.o: /usr/include/widec.h
display.o: /usr/include/sys/feature_tests.h
display.o: /usr/include/stdio.h
display.o: /usr/include/euc.h
display.o: /usr/include/sys/euc.h
display.o: /usr/include/wchar.h
display.o: /usr/include/sys/feature_tests.h
display.o: /usr/include/iso/wchar_iso.h
display.o: /usr/include/sys/feature_tests.h
display.o: /usr/include/stdio_tag.h
display.o: /usr/include/wchar_impl.h
display.o: /usr/include/sys/isa_defs.h
display.o: /usr/include/iso/time_iso.h
display.o: /usr/include/sys/feature_tests.h
display.o: /usr/include/stdio.h
display.o: /usr/include/ctype.h
display.o: /usr/include/iso/ctype_iso.h
display.o: /usr/include/sys/feature_tests.h
display.o: /usr/include/iso/ctype_c99.h
display.o: /usr/include/stddef.h
display.o: /usr/include/sys/isa_defs.h
display.o: /usr/include/iso/stddef_iso.h
display.o: /usr/include/sys/isa_defs.h
display.o: /usr/include/sys/va_list.h
display.o: /usr/include/iso/wchar_c99.h
display.o: /usr/include/stdio.h
display.o: /usr/include/termio.h
display.o: /usr/include/sys/termio.h
display.o: /usr/include/sys/termios.h
display.o: /usr/include/sys/feature_tests.h
display.o: /usr/include/sys/ttydev.h
display.o: /usr/include/sys/time.h
display.o: /usr/include/sys/feature_tests.h
display.o: /usr/include/sys/types.h
display.o: /usr/include/sys/feature_tests.h
display.o: /usr/include/sys/isa_defs.h
display.o: /usr/include/sys/machtypes.h
display.o: /usr/include/sys/feature_tests.h
display.o: /usr/include/sys/int_types.h
display.o: /usr/include/sys/feature_tests.h
display.o: /usr/include/sys/select.h
display.o: /usr/include/sys/feature_tests.h
display.o: /usr/include/sys/time_impl.h
display.o: /usr/include/sys/feature_tests.h
display.o: /usr/include/sys/time.h
display.o: /usr/include/time.h
display.o: /usr/include/sys/feature_tests.h
display.o: /usr/include/iso/time_iso.h
display.o: /usr/include/sys/types.h
display.o: /usr/include/sys/time_impl.h
display.o: /usr/include/sys/select.h
display.o: /usr/include/sys/types.h
display.o: /usr/include/unctrl.h
display.o: /usr/include/sys/types.h
display.o: /usr/include/sys/ipc.h
display.o: /usr/include/sys/isa_defs.h
display.o: /usr/include/sys/feature_tests.h
display.o: /usr/include/sys/types.h
display.o: /usr/include/sys/shm.h
display.o: /usr/include/sys/ipc.h
display.o: /usr/include/sys/unistd.h
display.o: /usr/include/sys/feature_tests.h
