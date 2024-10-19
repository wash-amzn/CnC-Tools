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
    char (*columnNames)[256];
    double *resultList;
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

    if (fgets(buffer, BUFFERLIMIT, file) == NULL)
        return data;

    //Parse first line for metadata
    if(VERSIONCODE != atoi(strtok_r(buffer, ",", &bufferSave))) return data;
    data.resultCount = atoi(strtok_r(bufferSave, ",", &bufferSave));
    data.columnCount = atoi(strtok_r(bufferSave, ",", &bufferSave));
    if((data.resultCount == 0) || (data.columnCount == 0)) return data;

    data.columnNames = malloc(data.columnCount * sizeof(char[255]));
    fgets(buffer, BUFFERLIMIT, file);
    bufferSave = buffer;

    // Grab each column name
    for (int i = 0; i < data.columnCount; i++) {
        char *pos = strtok_r(bufferSave, ",", &bufferSave);
        int len = strlen(pos);
        // Prevent oversized names, and remove newline
        if (len > 255)
            len = 255;
        else if (pos[len - 1] == '\n')
            len -= 1;
        memcpy(data.columnNames[i], pos, len);
        // Ensure it's null terminated
        data.columnNames[i][len] = 0;
    }

    data.resultList = malloc(data.resultCount * sizeof(double));
    fgets(buffer, BUFFERLIMIT, file);
    bufferSave = buffer;

    //Grab one line at a time and parse into values.  Currently limited to FP64 types
    for (int i = 0; i < data.resultCount; i++) {
        data.resultList[i] = atof(strtok_r(bufferSave, ",", &bufferSave));
    }

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

int write_CNC(char testName[], double resultList[], uint32_t resultCount, uint32_t columnCount, char *(columnNames[255]))
{
    //This block is unfinished and does not yet handle file extension or naming properly
    FILE *file;
    if((file = fopen(testName, "w")) == NULL) return -1;

    //Writes the file metadata to the first row of the csv
    fprintf(file, "%u,", VERSIONCODE);
    fprintf(file, "%u,", resultCount);
    fprintf(file, "%u\n", columnCount);

    //Writes the column names to the second row of the csv
    for(int i = 0; i < columnCount; i++)
    {
        fprintf(file, "%s", columnNames[i]);
        if (i + 1 != columnCount)
            fprintf(file, ",");
    }
    fwrite("\n", sizeof(char), 1, file);

    //Writes the resultList in csv format with columnCount enforcing the frequency of line breaks
    uint16_t columnCursor = 0;
    for(int i = 0; i < resultCount; i++)
    {
        fprintf(file, "%lf", resultList[i]);
        if(columnCursor < columnCount)
        {
            if (i + 1 != resultCount)
                fprintf(file, ",");
            columnCursor++;
        }
        else
        {
            fprintf(file, "\n");
            columnCursor = 0;
        }
    }

    fclose(file);
    return 0;
}
#endif // STORAGE_H
