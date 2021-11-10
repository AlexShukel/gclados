#include "messageUtils.h"
#include "stdlib.h"
#include "stdio.h"

char* ptfStandardErrorMessage(bool pass, char* usage, char* expected, char* received) {
    const struct PtfAnsiFlags expectedValueFlags = ptfColors.createFlags(1, ptfColors.foregroundColor(PTF_GREEN));
    const struct PtfAnsiFlags receivedValueFlags = ptfColors.createFlags(1, ptfColors.foregroundColor(PTF_RED));

    char* messageBuff = calloc(1024, sizeof(char));
    int offset = 0;

    if(usage != NULL) {
        char* expectedString = ptfColors.applyFlags("expected", expectedValueFlags);
        char* receivedString = ptfColors.applyFlags("received", receivedValueFlags);

        char* usageColorized = calloc(256, sizeof(char));

        sprintf(usageColorized, usage, receivedString);

        offset += sprintf(messageBuff, "  ensure(%s, %s);\n\n", expectedString, usageColorized);

        free(expectedString);
        free(receivedString);
        free(usageColorized);
    }

    if(pass) {
        char* expectedBuff = calloc(256, sizeof(char));
        sprintf(expectedBuff, "not %s", expected);
        char* expectedBuffColorized = ptfColors.applyFlags(expectedBuff, receivedValueFlags);
        offset += sprintf(messageBuff + offset, "    Expected: %s", expectedBuffColorized);

        free(expectedBuff);
        free(expectedBuffColorized);
    } else {
        char* expectedColorized = ptfColors.applyFlags(expected, expectedValueFlags);
        char* receivedColorized = ptfColors.applyFlags(received, receivedValueFlags);

        offset += sprintf(messageBuff + offset, "    Expected: %s\n    Received: %s", expectedColorized, receivedColorized);

        free(expectedColorized);
        free(receivedColorized);
    }

    return messageBuff;
}
