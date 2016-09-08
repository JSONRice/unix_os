/***************************************************************** 
 * Student: Jason Rice
 * 
 * Course: 605.414 System Development in a Unix Environment
 * 
 * File: getarg.c
 *
 * Description:
 *    This is the implementation files for the getarg 
 * header and contains the implementation for get_argument The 
 * name is self-explanatory. The function picks up the second 
 * argument from the command line and casts it to a double.
 ****************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "getarg.h"

int get_argument(int argc, char *argv[], double *return_value)
{
    int i = 0;
    size_t strlength = 0;
    int single_decimal = 0;
    if (argc >= 2)
    {
       // Check to see if input is valid.
       strlength = strlen(argv[1]);
       if (strlength <= 0)
       {
          return ERROR;
       }
       for (; i < strlength; ++i)
       {
          if(!isdigit(argv[1][i])){
             // Allow for negative numbers
             if (i == 0 && argv[1][i] == '-'){
                continue;
             }
             // Allow for a single decimal point
				 else if(argv[1][i] == '.' && !single_decimal){
					 single_decimal = 1;
					 continue;
				 }
             // Input is not a valid floating point (precision) number.
				 return ERROR;
          }
       }
       // At this point the input is valid. Assign to return value.
       *return_value = atof(argv[1]);   
       return OK;    
    }
    // Not enough arguments!
    return ERROR;
}
