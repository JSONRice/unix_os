########################################################
# Author : Student 2
# Desc.  : Calls the makefile from it lower level
# Date   : October 2
########################################################

SUB = src

it install clean depend:
	-X=`pwd`; \
	for i in ${SUB}; \
	do echo "### $$i ###"; cd $$X/$$i; make $@; done
