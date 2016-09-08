
SRC	= log.c
OBJ	= log.o
DEST	= ../../../lib

.c.o:
	cc -I../../../include -c $<

it:	liblog.a

install: ../liblog.a
../liblog.a : liblog.a
	cp liblog.a $(DEST)
	chmod 664 $(DEST)/liblog.a
	ranlib $(DEST)/liblog.a

liblog.a: $(OBJ)
	rm -f liblog.a
	ar cr liblog.a $(OBJ)

clean cleanup:
	rm -f *.o core

bare: clean
	rm -f liblog.a tags

depend:
	
