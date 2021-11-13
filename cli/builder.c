#include "builder.h"
#include "stdio.h"
#include "ptf.h"
#include "string.h"

#define STR_VALUE(arg)      #arg
#define MACRO_VALUE(name) STR_VALUE(name)
#define PTF_TEST_PREFIX_AS_STRING MACRO_VALUE(PTF_TEST_PREFIX)

char *buildTestFile(struct ParsedTestFile* testFiles, size_t count) {
    char *filename = tmpNameExtended(".c");

    if(filename == NULL) {
        // TODO: call panic here
    }

    FILE* outputFile = fopen(filename, "w");

    fprintf(outputFile, "#include \"ptf.h\"\n");

    for(int i = 0; i < count; ++i) {
        for(int j = 0; j < testFiles[i].testCount; ++j) {
            fprintf(outputFile,
                    "extern struct PtfTest %s%s(char *description);\n",
                    PTF_TEST_PREFIX_AS_STRING,
                    testFiles[i].tests[j].name);
        }
    }

    fprintf(outputFile, "\nint main() {\n");

    for(int i = 0; i < count; ++i) {
        fprintf(outputFile, "    struct PtfTest ptfTests%d[] = {\n", i);
        for(int j = 0; j < testFiles[i].testCount; ++j) {
            fprintf(outputFile,
                    "        %s%s(\"%s\"),\n",
                    PTF_TEST_PREFIX_AS_STRING,
                    testFiles[i].tests[j].name,
                    testFiles[i].tests[j].description == NULL ? testFiles[i].tests[j].name : testFiles[i].tests[j].description);
        }
        fprintf(outputFile, "    };\n");
        fprintf(outputFile,
                "    struct PtfTestSuite ptfTestSuite%d = createPtfTestSuite(\"%s\", ptfTests%d, %ld);\n",
                i,
                testFiles[i].fileName,
                i,
                testFiles[i].testCount);
    }

    fprintf(outputFile, "    struct PtfTestSuite ptfTestSuites[] = {\n");
    for(int i = 0; i < count; ++i) {
        fprintf(outputFile, "        ptfTestSuite%d,\n", i);
    }
    fprintf(outputFile, "    };\n    runPtfTestSuites(ptfTestSuites, %ld);\n", count);

    fprintf(outputFile, "}\n");

    fclose(outputFile);

    return filename;
}
