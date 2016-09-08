
BIN = ../../../bin
SRC = probe.c explore.c maze.c sleepms_select.c
OBJ = probe.o explore.o maze.o sleepms_select.o
INCL = ../../../include
SOLARISFLAGS = -lsocket -lnsl

.c.o:
	cc -g -I$(INCL) -c $<

it: probe

probe: $(OBJ)
	cc -g -o probe $(OBJ) -L../../../lib -llog $(SOLARISFLAGS)

tmaze: $(OBJ)
	cc -Bstatic -g -o tmaze maze.o tmaze.c -L../../../lib -llog $(SOLARISFLAGS)

$(BIN)/probe : probe
install: $(BIN)/probe
	rm -f $(BIN)/probe 
	cp probe $(BIN)

clean bare: 		
	rm -f *.o core probe maze

depend:
	rm -f dependlist
	gcc -I$(INCL) -M $(SRC) > dependlist

include dependlist
