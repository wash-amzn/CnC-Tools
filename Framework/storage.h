#ifndef STORAGE_H
#define STORAGE_H
/*
 * Program Name: CnC Common Headers
 * File Name: storage.h
 * Date Created: February 4, 2024
 * Date Updated: August 31, 2024
 * Version: 0.2
 * Purpose: Provides a struct for storing results and functions for file logging
 */
#include <stdio.h>
#include <stdint.h>

struct headerEntry
{
    char headerTag[13] = "Chips&Cheese";
    uint16_t columns;
    //uint32_t rows;
};


/*
 * Writes the header of the csv, which defines table names and certain formatting rules.
 */

int write_CNC_header(char testName[])
{

}

/*
 * Writes the stored list of results to a csv file
 * @Param TestName: a char array representing the name of the test.
 * @Param resultList: an array of resultEntry structs.
 * @Param resultCount: The length of the resultList array passed in.
 * @Return: 0 if successful
 */

int write_CNC_contents(char testName[], uint64_t resultList[], int resultCount)
{
    return 0;
}
#endif // STORAGE_H
