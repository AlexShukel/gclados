// Author: Artiom Tretjakovas
// Description: This file contains implementations of main functions. Function prototypes are defined in "gclados.h"
//              file.

#include "gclados.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "ioutils.h"

const char *GCLADOS_SUMMARY_ROW_FORMAT = "%12s: ";

void gcladosDrawSuites(const GcladosTestSuite suites[],
                       const GcladosTestSuiteAccumulatedResult results[],
                       size_t count,
                       bool minified) {
    fflush(stdout);
    // Clear screen
    printf("\033c");

    for(size_t i = 0; i < count; ++i) {
        gcladosPrintSuite(suites[i], results[i]);

        if(!minified && results[i].status == GCLADOS_FAILED) {
            for(size_t j = 0; j < results[i].testResultCount; ++j) {
                gcladosPrintTest(suites[i].tests[j], results[i].testResults[j]);
            }
        }
    }
}

void gcladosPrintSummary(char *name, size_t total, size_t passed) {
    printf(GCLADOS_SUMMARY_ROW_FORMAT, name);

    if(passed < total) {
        char buffer[30];
        sprintf(buffer, "%ld failed", total - passed);
        GcladosAnsiFlags flags =
                gcladosColors.createFlags(2, gcladosColors.foregroundColor(GCLADOS_RED), gcladosColors.bold());
        char *coloredOutput = gcladosColors.applyFlags(buffer, flags);
        printf("%s, ", coloredOutput);
        gcladosColors.freeFlags(flags);
        free(coloredOutput);
    }

    if(passed > 0) {
        char buffer[30];
        sprintf(buffer, "%ld passed", passed);
        GcladosAnsiFlags flags =
                gcladosColors.createFlags(2, gcladosColors.foregroundColor(GCLADOS_GREEN), gcladosColors.bold());
        char *coloredOutput = gcladosColors.applyFlags(buffer, flags);
        printf("%s, ", coloredOutput);
        gcladosColors.freeFlags(flags);
        free(coloredOutput);
    }

    printf("%ld total\n", total);
}

int gcladosRunTestSuites(GcladosTestSuite *suites, size_t count) {
    size_t completedTestSuites = 0;

    GcladosTestSuiteAccumulatedResult *results = calloc(count, sizeof(GcladosTestSuiteAccumulatedResult));

    const clock_t startTime = clock();

    while(completedTestSuites < count) {
        size_t currentSuiteIndex = completedTestSuites;

        results[currentSuiteIndex] = gcladosInitializeTestSuiteResult(suites[currentSuiteIndex]);
        results[currentSuiteIndex].status = GCLADOS_RUNNING;

        bool suiteCompleted = false;

        while(!suiteCompleted) {
            gcladosDrawSuites(suites, results, count, true);
            suiteCompleted = gcladosRunNextTest(suites[currentSuiteIndex], &results[currentSuiteIndex]);
        }

        ++completedTestSuites;
        gcladosCompleteTestSuiteResult(&results[currentSuiteIndex]);
    }

    const clock_t endTime = clock();

    gcladosDrawSuites(suites, results, count, false);

    size_t totalTests = 0, passedTests = 0;
    size_t totalTestSuites = count, passedTestSuites = 0;

    for(size_t i = 0; i < count; ++i) {
        passedTestSuites += results[i].status == GCLADOS_PASS;
        totalTests += suites[i].testCount;
        for(size_t j = 0; j < results[i].testResultCount; ++j) {
            passedTests += results[i].testResults[j].pass == true;
        }
    }

    gcladosPrintSummary("Test suites", totalTestSuites, passedTestSuites);
    gcladosPrintSummary("Tests", totalTests, passedTests);
    printf(GCLADOS_SUMMARY_ROW_FORMAT, "Time");
    gcladosPrintTime(endTime - startTime);
    printf("\n");

    for(size_t i = 0; i < count; ++i) {
        gcladosFreeTestSuiteResult(results[i]);
    }

    return totalTests == passedTests ? EXIT_SUCCESS : EXIT_FAILURE;
}
