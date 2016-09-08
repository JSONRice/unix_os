########################################################
# Author : Student 2
# Desc.  : Calls the makefile from it lower level
# Date   : 
########################################################

SUB = log_mgr shmlib socketlib 

it install clean bare depend:
	-X=`pwd`; \
	for i in ${SUB}; \
	do echo "### $$i ###"; cd $$X/$$i; make $@; done
