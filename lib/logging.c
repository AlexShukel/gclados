#include "logging.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "panic.h"

#include "string.h"

FILE *logfile;

void gcladosEndLogging() {
    fclose(logfile);
}

void gcladosLog(char *logEntry) {
    if(logfile == NULL) {
        logfile = fopen("gclados.log", "a");
        if(logfile == NULL) {
            gcladosPanic("Could not create log file", EXIT_FAILURE);
        }
        atexit(gcladosEndLogging);
    }

    time_t rawTime;
    struct tm *timeInfo;
    time(&rawTime);
    timeInfo = localtime(&rawTime);
    char *timeString = asctime(timeInfo);
    timeString[strlen(timeString) - 1] = '\0';
    fprintf(logfile, "[%s] %s\n", timeString, logEntry);
}
