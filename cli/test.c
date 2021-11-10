#include "test.h"
#include "stdio.h"
#include "stdbool.h"

const char PTF_TEST_BEGIN_TOKEN[] = "TEST(";
const int PTF_TEST_BEGIN_TOKEN_SIZE = (sizeof(PTF_TEST_BEGIN_TOKEN) / sizeof(char)) - 1;

void readTestFile(char* path) {
    FILE* file = fopen(path, "r");

    int value;

    bool isInString = false;
    bool isReadingTestName = false;
    int readingTokenIndex = 0;

    char testNames[100][100];
    int currentTestIndex = 0;
    int currentTestNameIndex = 0;

    while(value = fgetc(file), value != EOF) {
        if(readingTokenIndex == PTF_TEST_BEGIN_TOKEN_SIZE) {
            isReadingTestName = true;
            readingTokenIndex = 0;
        }

        if(isReadingTestName && value == ')') {
            isReadingTestName = false;
            testNames[currentTestIndex][currentTestNameIndex] = '\0';
            ++currentTestIndex;
            currentTestNameIndex = 0;
        } else if(isReadingTestName) {
            testNames[currentTestIndex][currentTestNameIndex] = (char) value;
            ++currentTestNameIndex;
        } else if(value == '\"') {
            isInString = !isInString;
        } else if(value == PTF_TEST_BEGIN_TOKEN[readingTokenIndex]) {
            ++readingTokenIndex;
        } else {
            readingTokenIndex = 0;
        }
    }

    for(int i = 0; i < currentTestIndex; i++) {
        printf("%s\n", testNames[i]);
    }

    fclose(file);

}
