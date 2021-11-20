#include "testParser.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char PTR_TEST_TOKEN[] = "TEST(";
const int PTR_TEST_TOKEN_LENGTH = sizeof(PTR_TEST_TOKEN) / sizeof(char);
const int testFileBufferSize = 1024;

bool isToken(const char *ptr) {

    for(int i = 0; i < PTR_TEST_TOKEN_LENGTH - 1; i++) {
        if(ptr[i] != PTR_TEST_TOKEN[i]) {
            return false;
        }
    }

    return true;
}

void trimWhitespaces(char *string) {
    int begin = 0;
    int end = 0;

    for(int i = 0; i < strlen(string); i++) {
        if(isspace(string[i]) && end == 0) {
            begin = i;
        }

        if(!isspace(string[i])) {
            end = i;
        }
    }

    string[end + 1] = '\0';
    memmove(string, string + begin + 1, end - begin + 2);
}

char *extractComment(const char *buffer, size_t begin, size_t end) {
    const size_t commentLength = end - begin + 1;

    if(commentLength <= 1 || begin == -1 || end == -1) {
        return NULL;
    }

    char *comment = calloc(commentLength, sizeof(char));
    memcpy(comment, buffer + begin, (end - begin) * sizeof(char));
    trimWhitespaces(comment);

    return comment;
}

int processBuffer(const char buffer[testFileBufferSize], size_t bytesRead, ParsedTest *output) {
    size_t lastIndex = bytesRead - 5;

    int currentTest = 0;

    size_t lastCommentBegin = -1, lastCommentEnd = -1;

    for(size_t i = 1; i < lastIndex; ++i) {
        if(buffer[i] == '/' && buffer[i + 1] == '/') {
            lastCommentBegin = i + 2;

            bool wasEscaped = false;
            while(i < lastIndex && !(!wasEscaped && buffer[i] == '\n')) {
                wasEscaped = buffer[i] == '\\' || (wasEscaped && isspace(buffer[i]));
                ++i;
            }

            lastCommentEnd = i;
        } else if(buffer[i] == '/' && buffer[i + 1] == '*') {
            lastCommentBegin = i;
            ++i;

            while(i < lastIndex && !(buffer[i - 1] == '*' && buffer[i] == '/'))
                ++i;

            lastCommentEnd = i;
        } else if(buffer[i] == '"') {
            ++i;
            while(i < lastIndex && buffer[i] != '"')
                i += 1 + (buffer[i] == '\\');
        } else if(isspace(buffer[i - 1]) && isToken(buffer + i)) {
            i += PTR_TEST_TOKEN_LENGTH - 1;

            size_t beginPoint = i;

            while(i < lastIndex && buffer[i] != ')')
                ++i;

            size_t endPoint = i;

            char *name = calloc(endPoint - beginPoint + 1, sizeof(char));
            memcpy(name, buffer + beginPoint, (endPoint - beginPoint) * sizeof(char));

            ParsedTest newTest = {
                    .name = name,
                    .description = extractComment(buffer, lastCommentBegin, lastCommentEnd),
            };

            lastCommentBegin = lastCommentEnd = -1;

            output[currentTest++] = newTest;
        } else if(!isspace(buffer[i])) {
            lastCommentBegin = lastCommentEnd = -1;
        }
    }

    return currentTest;
}

ParsedTestFile parseTestFile(char *path) {
    FILE *file = fopen(path, "r");

    ParsedTest *tests = calloc(100, sizeof(ParsedTest));

    char buffer[testFileBufferSize];
    size_t bytesRead = fread(buffer, sizeof(char), testFileBufferSize, file);

    size_t count = processBuffer(buffer, bytesRead, tests);

    ParsedTestFile testFile = {
            .fileName = path,
            .testCount = count,
            .tests = tests,
    };

    return testFile;
}
