/**
 * @file test.h
 * @author Laiba Mustafa
 * @date May 2018
 * @brief File containing the function definitions for testing
 */


#ifndef TEST_HELPERS_H
#define TEST_HELPERS_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

/**
 Summary: Prints whether result passed or failed a test.
 @return boolean value of the parameter passed in
 @param boolean value indicating whether a test has passed or failed.  Used to choose what to print.
 @pre None
 @post Statement is printed depending on the value of result. Result is returned.
*/
int printPassFail( const int result );

/**
 Summary: Prints basic information about what expectation and result of a test. Only works for  strings.
 @pre None
 @param the number of the test that is being performed
 @param  the text to be printed about the test that will be performed
 @post Introduction Statement is printed, result printed depends on the value of result.
*/
void printTestIntroInfo( int testNumber, const char *intro);

/*
 Summary: Creats a message wrapped in star(*) for menu-type feel
 @param the text to be printed in the menu-type wrapper
 @pre None
 @post Statement is printed encapsulated by *
*/
void printStatement( const char * statement );

#endif //Ending TEST_HELPERS_H
