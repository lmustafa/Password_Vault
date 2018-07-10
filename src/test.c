/**
 * @file test.h
 * @author Laiba Mustafa
 * @date May 2018
 * @brief File containing the function implementations for testing
 */



#include "test.h"

int printPassFail( const int result )
{
 if( result )
   printf("Result:Passed\n\n");
 else
   printf("Result:Failed\n\n");
 return result;
}

void printTestIntroInfo( int testNumber, const char *intro)
{
 printf("Test #%d\n", testNumber);
 printf("Testing: %s\n", intro );
}

void printStatement( const char * statement )
{
 printf("************************** %s **************************\n",statement);
}
