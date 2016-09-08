#########################################################################
# Student: Jason Rice
#
# Course: 605.414 Systems Development in a Unix Environment
#
# File: Makefile
#
# Description:
#    This Makefile acts as a recursive root level initializer by calling
# the Makefiles within each directory below the current working 
# directory. Those Makefiles within directories containing other 
# Makefiles will then make a similar recursive call.
#########################################################################
SUB = src
PWD = `pwd`

# The '@' suppresses the annoying output of the for loop syntax so
# programmers don't have to see it every time they run make.
it install depend clean:
	@echo -e "\n Running target '$@'";
	@for i in $(SUB); \
	do echo -e "\n**************************************************"; \
	echo "<<< $$i >>>"; cd $(PWD)/$$i; make $@; cd -; \
	echo -e "\n**************************************************"; done
