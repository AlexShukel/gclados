#include "builder.h"
#include "stdio.h"
#include "ptf.h"

#define STR_VALUE(arg)      #arg
#define MACRO_VALUE(name) STR_VALUE(name)
#define PTF_TEST_PREFIX_AS_STRING MACRO_VALUE(PTF_TEST_PREFIX)

char *buildTestFile(struct ParsedTest* tests, size_t count) {
    char *filename = calloc(L_tmpnam, sizeof(char));

    if(tmpnam(filename) == NULL) {
        // TODO: call panic here
    }

    FILE* outputFile = fopen(filename, "w");

    fprintf(outputFile, "#include \"ptf.h\"\n");

    for(int i = 0; i < count; i++) {
        fprintf(outputFile, "extern struct PtfTest %s%s(char *description);\n", PTF_TEST_PREFIX_AS_STRING, tests[i].name);
    }

    fprintf(outputFile, "\nint main() {\n");

    for(int i = 0; i < count; i++) {
        fprintf(outputFile, "    %s%s(\"%s\").execute();\n", PTF_TEST_PREFIX_AS_STRING, tests[i].name, tests[i].description);
    }

    fprintf(outputFile, "}\n");

    fclose(outputFile);

    return filename;
}
