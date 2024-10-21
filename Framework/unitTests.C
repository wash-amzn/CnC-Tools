/*
 * Program Name: CnC Framework Unit Tests
 * File Name: unitTests.c
 * Date Created: October 19, 2024
 * Date Updated: October 21, 2024
 * Version: 0.2
 * Purpose: Unit Tests for the Framework
 */

#include <storage.h>

/*
 * Test the storage API's ability to read and write accurately
 * @Param ifile: Name for the input file being tested.
 * @Return: 0 if successful, -1 for IO error, -2 for verification failure
 */

int testStorage(char *ifile) {
    CnCData data = read_CNC(ifile);
    if (write_CNC("writeback_test", data.resultList, data.resultCount, data.columnCount, data.columnNames) != 0)
        return -1;

    CnCData data_copy = read_CNC("writeback_test");

    // Compare the 2 versions of the data
    if (data.columnCount != data_copy.columnCount)
        return -2;
    for (int i = 0; i < data.columnCount; i++)
        if (strcmp(data.columnNames[i], data_copy.columnNames[i]))
            return -2;
    if (data.resultCount != data_copy.resultCount)
        return -2;
    for (int i = 0; i < data.resultCount; i++)
        if (data.resultList[i] != data_copy.resultList[i])
            return -2;
    if (data.isMalformed != 0 & data_copy.isMalformed != 0)
        return -2;

    return 0;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Please provide a test file for the storage test\n");
        return -1;
    }

    int result = 0;

    printf("Storage test exitted with return code %i\n", result |= testStorage(argv[1]));

    return 0;
}