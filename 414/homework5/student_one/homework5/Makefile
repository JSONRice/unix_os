# 
# homework 3, Unix Systems Programming
# Fall
#
# file: Makefile
#    This makefile is based on the class example.  It recognizes it, 
# install, clean, and depend as targets.  
#    Note, it is not necessary to call make depend after a make clean 
# because the dependancies are automatically generated if they are 
# missing.  See lower level makefiles for the details.
#    This makefile uses borne shell commands to loop through sub-
# directories and executes the makefiles contained within.


SUBS = src  

#note, with one subdirectory, the loop is not needed, but does not hurt.

it install clean depend:
	-@ X=`pwd`; \
	for i in $(SUBS); \
	do echo '<<<' $$i '>>>'; cd $$X/$$i; make $@; done
	
cleanall: clean
	rm ./bin/control_probe ./bin/probe ./bin/display
	rm ./lib/liblog_mgr.a ./lib/liblog.a ./lib/libsock.a ./lib/shmlib.a
