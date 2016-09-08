/***************************************************************** 
 * Student: Jason Rice
 * 
 * Course: 605.414 System Development in a Unix Environment
 * 
 * File: fahr_convert.c
 *
 * Description:
 *    This is one of the implementation files for the temp_conv 
 * header and contains the implementation for convert_to_fahr
 * The name is self-explanatory. The function takes a double
 * of centigrade and converts it (returns) the Fahrenheit.
 ****************************************************************/

double convert_to_fahr(double cent){
    return (1.80 * cent) + 32.00;
}
