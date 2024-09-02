#ifndef STORAGE_H
#define STORAGE_H
/*
 * Program Name: CnC Common Headers
 * File Name: storage.h
 * Date Created: February 4, 2024
 * Date Updated: September 2, 2024
 * Version: 0.2
 * Purpose: Provides a struct for storing results and functions for file logging
 */
#include <stdio.h>
#include <stdint.h>

struct CnCData
{
    char testName[255];
    uint32_t resultCount;
    uint16_t columns;
    uint64_t resultList[];
};



/*
 * Writes the stored list of results to a csv file with the .cnc extension
 * @Param TestName: a char array representing the name of the test.
 * @Param resultList: an array of results.
 * @Param resultCount: The length of the resultList array passed in.
 * @Param columnCount: The number of columns per line
 * @Return: 0 if successful, -1 for IO error
 */

int write_CNC(char testName[], uint64_t resultList[], int resultCount, uint16_t columnCount)
{
    //This block is unfinished and does not yet handle file extension or naming properly
    FILE *file;
    if((file = fopen(testName, "w")) == NULL) return -1;
    uint16_t columnCursor = 0;

    //Writes the resultList in csv format with columnCount enforcing the frequency of line breaks
    for(int i = 0; i < resultCount; i++)
    {
        fwrite(&resultList[i], sizeof(uint64_t), 1, file);
        if(columnCursor < columnCount)
        {
            fwrite(",", sizeof(char), 1, file);
            columnCursor++;
        }
        else
        {
            fwrite("\n", sizeof(char), 1, file);
            columnCursor = 0;
        }
    }

    fclose(file);
    return 0;
}
#endif // STORAGE_H
