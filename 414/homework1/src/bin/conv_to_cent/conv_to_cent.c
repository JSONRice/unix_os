/***************************************************************** 
 * Student: Jason Rice
 * 
 * Course: 605.414 System Development in a Unix Environment
 * 
 * File: conv_to_cent.c
 *
 * Description:
 *    This program converts Fahrenheit to centigrade and utilizes
 * the proper temperature conversion libraries built via 'make'
 ****************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "temp_conv.h"

void output(const double inval, const double outval)
{
   printf("\n%.2f degrees Fahrenheit is %.2f degrees centigrade.\n\n", inval, outval);
}

/****************************************************************
 * Main calls the functions to perform the calculation from
 * Fahrenheit to centigrade. It does so by calling the 
 * convert_to_cent function from the temp_conv library. Main also
 * makes a call to the get_argument function within the arg library
 * in order to pick up the user input from the command line.
 ***************************************************************/
main(int argc, char *argv[])
{
   double zero = 0;
   double *in = &zero;
   if (get_argument(argc, argv, in) == -1)
   {
      printf("\tusage: conv_to_cent <centigrade number>\n"); 
      exit(1);
   }
   double result = convert_to_cent(*in);
   output(*in, result);
   exit(0);
}
