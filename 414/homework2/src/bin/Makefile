#########################################################################
# Student: Jason Rice
#
# Course: 605.414 Systems Development in a Unix Environment
#
# File: Makefile
#
# Description:
#   This Makefile acts as a recursive call to the Makefiles within each
# directory below the current working directory and is similar in 
# structure to root level Makefile.
#########################################################################
SUB = test_log test_log2 lots_of_logs
PWD = `pwd`

# The '@' suppresses the annoying output of the for loop syntax so
# programmers don't have to see it every time they run make.
it install depend clean:
	@for i in $(SUB); \
	do echo -e "\n**************************************************"; \
	echo "<<< $$i >>>"; cd $(PWD)/$$i; make $@; cd -; \
	echo -e "\n**************************************************"; done
