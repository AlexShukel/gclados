// Author: Artiom Tretjakovas
// Description: File containing the implementation of the entry tests file building function. Function prototype
//              described in header file (builder.h)

#include "builder.h"

#include <stdio.h>
#include <string.h>

#include "gclados.h"

#define STR_VALUE(arg) #arg
#define MACRO_VALUE(name) STR_VALUE(name)
#define GCLADOS_TEST_PREFIX_AS_STRING MACRO_VALUE(GCLADOS_TEST_PREFIX)

int buildTestFile(FILE *outputFile, const ParsedTestFile testFiles[], size_t count) {
    // Including required modules at the file beginning.
    fprintf(outputFile, "#include \"gclados.h\"\n#include <stdbool.h>\n");

    // Generating test functions' declarations.
    for(int i = 0; i < count; ++i) {
        for(int j = 0; j < testFiles[i].testCount; ++j) {
            fprintf(outputFile,
                    "extern GcladosTest %s%s(char *description);\n",
                    GCLADOS_TEST_PREFIX_AS_STRING,
                    testFiles[i].tests[j].name);
        }
    }

    // Main method.
    fprintf(outputFile, "\nint main() {\n");
    // Toggling color support.
    fprintf(outputFile,
            "    gcladosColors.setColorsSupported(%s);\n",
            gcladosColors.colorsSupported() ? "true" : "false");

    for(int i = 0; i < count; ++i) {
        // Generating array of tests.
        fprintf(outputFile, "    GcladosTest gcladosTests%d[] = {\n", i);
        for(int j = 0; j < testFiles[i].testCount; ++j) {
            fprintf(outputFile,
                    "        %s%s(\"%s\"),\n",
                    GCLADOS_TEST_PREFIX_AS_STRING,
                    testFiles[i].tests[j].name,
                    testFiles[i].tests[j].description == NULL ? testFiles[i].tests[j].name
                                                              : testFiles[i].tests[j].description);
        }
        fprintf(outputFile, "    };\n");
        // Generating test suite.
        fprintf(outputFile,
                "    GcladosTestSuite gcladosTestSuite%d = gcladosCreateTestSuite(\"%s\", gcladosTests%d, %ld);\n",
                i,
                testFiles[i].fileName,
                i,
                testFiles[i].testCount);
    }

    // Constructing test suite array, to execute them later.
    fprintf(outputFile, "    GcladosTestSuite gcladosTestSuites[] = {\n");
    for(int i = 0; i < count; ++i) {
        fprintf(outputFile, "        gcladosTestSuite%d,\n", i);
    }
    // Executing test suites.
    fprintf(outputFile, "    };\n    return gcladosRunTestSuites(gcladosTestSuites, %ld);\n", count);

    fprintf(outputFile, "}\n");

    return 0;
}

#undef STR_VALUE
#undef MACRO_VALUE
#undef GCLADOS_TEST_PREFIX_AS_STRING
