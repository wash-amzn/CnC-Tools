#ifndef STORAGE_H
#define STORAGE_H
/*
 * Program Name: CnC Common Headers
 * File Name: storage.h
 * Date Created: February 4, 2024
 * Date Updated: October 19, 2024
 * Version: 0.2.1
 * Purpose: Provides a struct for storing results and functions for file logging
 */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Currently the ".cnc" file format is a modified csv with a header row, followed by a
 * single row of strings representing the column names, and then all subsequent rows are the data entry points.
 *
 * Header Row contains 3 metadata entries corresponding to the version number, the result count, and the column count.
 *
 * LIMITATIONS: Currently column names are limited to 255 characters (no commas allowed), and all results are formatted as double precision FP.
 * Differing text and line encoding are also currently untested and potentially unsupported.
 */

#define BUFFERLIMIT 100000 //The max value for input and other read operations

const uint8_t VERSIONCODE = 1;


typedef struct __CnCData
{
    uint8_t isMalformed;
    char testName[255];
    uint32_t resultCount;
    uint32_t columnCount;
    double resultList[];
} CnCData; 

/*
 * Reads/parses the specified file and produces a CnCData struct from the data entries.
 * @Param fileName: A char array representing the name of the file to be ingested.
 * @return: a single CnCData struct.  testName = FAILED if the test does not parse successfully.
 */
CnCData read_CNC(char fileName[])
{
    CnCData data = { .isMalformed = 1 };
    char buffer[BUFFERLIMIT];
    char *bufferSave;
    sprintf(data.testName, "FAILED"); //Default value is the fail condition

    FILE *file;
    if((file = fopen(fileName, "r")) == NULL) return data;

    fgets(buffer, BUFFERLIMIT, file);

    //Parse first line for metadata
    if(VERSIONCODE != atoi(strtok_r(buffer, ",", &bufferSave))) return data;
    data.resultCount = (uint32_t) atoi(strtok_r(NULL, ",", &bufferSave));
    data.columnCount = (uint32_t) atoi(strtok_r(NULL, ",", &bufferSave));
    if((data.resultCount == 0) || (data.columnCount == 0)) return data;

    //Grab one line at a time and parse into values.  Currently limited to FP64 types

    fclose(file);
    data.isMalformed = 0; //Sets the malform check to false as the operation is complete.

    return data;
}

/*
 * Writes the stored list of results to a csv file with the .cnc extension
 * @Param TestName: a char array representing the name of the test.
 * @Param resultList: an array of results.
 * @Param resultCount: The length of the resultList array passed in.
 * @Param columnCount: The number of columns per line
 * @Return: 0 if successful, -1 for IO error
 */

int write_CNC(char testName[], double resultList[], uint32_t resultCount, uint32_t columnCount, char columnNames[][255])
{
    //This block is unfinished and does not yet handle file extension or naming properly
    FILE *file;
    if((file = fopen(testName, "w")) == NULL) return -1;

    //Writes the file metadata to the first row of the csv
    fwrite(&VERSIONCODE, sizeof(VERSIONCODE), 1, file);
    fwrite(",", sizeof(char), 1, file);
    fwrite(&resultCount, sizeof(resultCount), 1, file);
    fwrite(",", sizeof(char), 1, file);
    fwrite(&columnCount, sizeof(columnCount), 1, file);
    fwrite("\n", sizeof(char), 1, file);



    //Writes the column names to the second row of the csv
    for(int i = 0; i < columnCount; i++)
    {
        fwrite(&columnNames[i][0], sizeof(char), 255, file);
        fwrite(",", sizeof(char), 1, file);
    }
    fwrite("\n", sizeof(char), 1, file);

    //Writes the resultList in csv format with columnCount enforcing the frequency of line breaks
    uint16_t columnCursor = 0;
    for(int i = 0; i < resultCount; i++)
    {
        fwrite(&resultList[i], sizeof(double), 1, file);
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
