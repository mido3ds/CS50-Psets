/**
*   to diagnose/handle errors/exceptions 
*   written by Mahmoud Adas
*   22/9/2016
*/


#ifndef ERROR_H
#define ERROR_H

#include <iostream>

#define SUCCESS 0

enum class ERROR 
{
    READ_FAILURE = 1,
    WRITE_FAILURE = 2,
    OPEN_FILE_FAILURE = 3,
    BAD_WRITTEN_LINE = 4
};

#endif