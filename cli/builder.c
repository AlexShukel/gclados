#include "builder.h"
#include "stdio.h"
#include "gclados.h"
#include "string.h"

#define STR_VALUE(arg)      #arg
#define MACRO_VALUE(name) STR_VALUE(name)
#define GCLADOS_TEST_PREFIX_AS_STRING MACRO_VALUE(GCLADOS_TEST_PREFIX)

char *buildTestFile(struct ParsedTestFile* testFiles, size_t count) {
    char *filename = tmpNameExtended(".c");

    if(filename == NULL) {
        gcladosPanic("Could not create temporary file for tests entrypoint.");
    }

    FILE* outputFile = fopen(filename, "w");

    fprintf(outputFile, "#include \"gclados.h\"\n");

    for(int i = 0; i < count; ++i) {
        for(int j = 0; j < testFiles[i].testCount; ++j) {
            fprintf(outputFile,
                    "GcladosTest %s%s(char *description);\n",
                    GCLADOS_TEST_PREFIX_AS_STRING,
                    testFiles[i].tests[j].name);
        }
    }

    fprintf(outputFile, "\nint main() {\n");

    for(int i = 0; i < count; ++i) {
        fprintf(outputFile, "    GcladosTest gcladosTests%d[] = {\n", i);
        for(int j = 0; j < testFiles[i].testCount; ++j) {
            fprintf(outputFile,
                    "        %s%s(\"%s\"),\n",
                    GCLADOS_TEST_PREFIX_AS_STRING,
                    testFiles[i].tests[j].name,
                    testFiles[i].tests[j].description == NULL ? testFiles[i].tests[j].name : testFiles[i].tests[j].description);
        }
        fprintf(outputFile, "    };\n");
        fprintf(outputFile,
                "    GcladosTestSuite gcladosTestSuite%d = gcladosCreateTestSuite(\"%s\", gcladosTests%d, %ld);\n",
                i,
                testFiles[i].fileName,
                i,
                testFiles[i].testCount);
    }

    fprintf(outputFile, "    GcladosTestSuite gcladosTestSuites[] = {\n");
    for(int i = 0; i < count; ++i) {
        fprintf(outputFile, "        gcladosTestSuite%d,\n", i);
    }
    fprintf(outputFile, "    };\n    gcladosRunTestSuites(gcladosTestSuites, %ld);\n", count);

    fprintf(outputFile, "}\n");

    fclose(outputFile);

    return filename;
}
