# 
# homework 4, Unix Systems Programming
# Fall
#
#    I broke the makefile up into several make files.  Makefile
# takes care of building the dependlist and it takes care of the
# clean target.  To make it and make install, this makefile will
# first verify the existence of a dependlist and then call a worker
# makefile.  
#    The worker makefile will actually build the target.  The worker
# makefile includes the depend list made Makefile.
#    Both Makefile and the worker makefile make use of a parameter
# makefile.  This lets the two makefile share the same macros and it
# also consolidates the macros for the user.  The user should not need
# to modify Makefile or the worker makefile.  The user only needs to 
# look at the parameters makefile.

include Makefile_params

makeDependlist = gcc -M ${CFLAGS} ${SRCS} > dependlist


it install: 
	@ if [ -f dependlist ] ; then cd . ; else $(makeDependlist); fi
	@ make -f Makefile_worker $@

depend:
	$(makeDependlist)

clean:
	rm *.o dependlist $(MAINTARGET)

