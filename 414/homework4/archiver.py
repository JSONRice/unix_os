#!/usr/bin/python
########################################################################
# Student: Jason Rice
#
# Course: 605.414 Systems Development in a Unix Environment
#
# File: archiver.py
#
# Description:
#    Python script that creates the tar files of the homework. There are
# two options for release, one being a binary release that archives the
# executables and a source release will archives the entire structure
# along with source code of the homework. Only one mode will be executed
# although the script allows for multiple modes to be set on the
# command line. If two modes are set the first mode will be carried
# out. The two modes are set on the command line as '-b' for binary 
# and '-s' for source.
########################################################################
import getopt;
import sys;
import subprocess;
import socket;

# Global host name from socket API
Host = socket.gethostname()

def usage():
    print("Usage:")
    print("   archiver.py [-b|-s]")
    print("Options:")
    print("   -b --> Archives target in binary mode.")
    print("   -s --> Archives target in source mode.")

def binary_archive():
    subprocess.call('make clean depend install', shell=True)
    subprocess.call('tar -cvf homework4_'+Host+'.tar ../homework4/bin/', shell=True)

def source_archive():
    subprocess.call('make clean', shell=True)
    subprocess.call('tar -cvf homework4.tar ../homework4/', shell=True)

def prompt(question, default="no"):
    valid = {"yes": True, "y": True, "ye": True,
             "no": False, "n": False}
    if default is None:
        prompt = " [y/n] "
    elif default == "yes":
        prompt = " [Y/n] "
    elif default == "no":
        prompt = " [y/N] "
    else:
        raise ValueError("invalid default answer: '%s'" % default)

    while True:
        sys.stdout.write(question + prompt)
        choice = raw_input().lower()
        if default is not None and choice == '':
            return valid[default]
        elif choice in valid:
            return valid[choice]
        else:
            sys.stdout.write("Please respond with 'yes' or 'no' "
                             "(or 'y' or 'n').\n")


def main():
    try:
        opts, args = getopt.getopt(sys.argv[1:], "bs")
    except getopt.GetoptError as err:
        # will print something like "option -a not recognized"
        print str(err)
        usage()
        sys.exit(1)
    binary = False
    source = False
    for o, a in opts:
        if o == "-b":
            binary = True
            break
        elif o == "-s":
            source = True
            break
    
    # Evaluate whether one or more of the modes is set and carry out archiving:
    if binary == True:
        print "Arhiving in binary mode."
        if prompt("Is this correct?:") == True:
           binary_archive()
           print "Archive generated."
        else:
           print "Operation aborted by user."
    elif source == True:
        print "Archiving in source mode."
        if prompt("Is this correct?:") == True:
           source_archive()
           print "Archive generated."
        else:
           print "Operation aborted by user."
    else:
        print "No valid mode set. Please review usage below."
        usage()

if __name__ == "__main__":
   main()
