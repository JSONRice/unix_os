/***************************************************************** 
 * Student: Jason Rice
 * 
 * Course: 605.414 System Development in a Unix Environment
 * 
 * File: conv_to_fahr.c
 *
 * Description:
 *    This program converts centigrade to Fahrenheit and utilizes
 * the proper temperature conversion libraries built via 'make'
 ****************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "temp_conv.h"

const int Max = 32;

void prompt()
{
   printf("\nEnter number of centigrade degrees: ");
}

const double getinput()
{
   char str[Max];
   fgets(str, sizeof(str), stdin);
   if (!inputvalid(str))
   {
      printf("Warning input is invalid. Please enter a positive number next time.\n");
      exit(0);
   }
   return atof(str);
}

int inputvalid(const char *str)
{
   int i = 0;
   int single_decimal = 0;
   // Count the length minus one to bypass the newline.
   size_t strlength = strlen(str) - 1;
   // Is there no input (e.g. user entered the 'return')?
   if (strlength <= 0)
   {
      return 0;
   }
   for (; i < strlength; ++i)
   {
      if (!isdigit(str[i]))
      {
         // Allow for negative numbers
	 if (i == 0 && str[i] == '-')
         { 
	    continue;
	 }
	 // Allow for a single decimal point
	 else if(str[i] == '.' && !single_decimal)
         {
	    single_decimal = 1;
	    continue;
	 }
	 return 0;
      }
   }
   return 1;
}

void output(const double inval, const double outval)
{
   printf("\n%.2f degrees centigrade is %.2f degrees Fahrenheit.\n\n", inval, outval);
}

/****************************************************************
 * Main calls the functions to perform the calculation from
 * centigrade to fahrenheit. It does so by calling the 
 * convert_to_fahr function from the temp_conv library.
 ***************************************************************/
main()
{
   prompt();
   double in = getinput();
   double result = convert_to_fahr(in);
   output(in, result);
   exit(0);
}
