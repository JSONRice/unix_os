/***************************************************************** 
 * Student: Jason Rice
 * 
 * Course: 605.414 System Development in a Unix Environment
 * 
 * File: getarg.h
 *
 * Description:
 *    This is the header file that declares the functions defined
 * within the getarg.c source file. The purpose is to retrieve the
 * command line arguments.
 ****************************************************************/
#ifndef GETARG_H
#define	GETARG_H

#define ERROR -1
#define OK 0

int get_argument(int argc, char *argv[], double *return_value);

#endif	/* GETARG_H */
