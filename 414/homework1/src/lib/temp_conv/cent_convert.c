/***************************************************************** 
 * Student: Jason Rice
 * 
 * Course: 605.414 System Development in a Unix Environment
 * 
 * File: cent_convert.c
 *
 * Description:
 *    This is one of the implementation files for the temp_conv 
 * header and contains the implementation for convert_to_cent
 * The name is self-explanatory. The function takes a double
 * of Fahrenheit and converts it (returns) the centigrade.
 ****************************************************************/

double convert_to_cent(double fahr){
   return (5.0/9.0) * (fahr-32.0);
}

