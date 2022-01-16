// Author: Artiom Tretjakovas
// Description: This file contains implementations of main functions. Function prototypes are defined in "gclados.h"
//              file.

#include "gclados.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "ioutils.h"
#include "logging.h"
#include "snapshots.h"

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

typedef enum
{
    GCLADOS_SUMMARY_SUCCESS,
    GCLADOS_SUMMARY_FAILURE,
    GCLADOS_SUMMARY_NEUTRAL,
} GcladosSummaryType;

typedef struct {
    size_t count;
    char *prefix;
    GcladosSummaryType type;
} GcladosSummary;

void gcladosPrintSummaryCell(GcladosSummary summary) {
    char *buffer = calloc(sizeof(char), 128);

    sprintf(buffer, "%lu %s", summary.count, summary.prefix);

    if(summary.type == GCLADOS_SUMMARY_NEUTRAL) {
        printf("%s", buffer);
    }

    if(summary.type == GCLADOS_SUMMARY_FAILURE) {
        GcladosAnsiFlags flags = gcladosDefaultFailureStyle();
        char *colorizedBuffer = gcladosColors.applyFlags(buffer, flags);
        printf("%s", colorizedBuffer);
        free(colorizedBuffer);
        gcladosColors.freeFlags(flags);
    }

    if(summary.type == GCLADOS_SUMMARY_SUCCESS) {
        GcladosAnsiFlags flags = gcladosDefaultSuccessStyle();
        char *colorizedBuffer = gcladosColors.applyFlags(buffer, flags);
        printf("%s", colorizedBuffer);
        free(colorizedBuffer);
        gcladosColors.freeFlags(flags);
    }

    free(buffer);
}

void gcladosPrintSummaryRow(char *name, GcladosSummary *summaries, size_t summaryCount) {
    printf(GCLADOS_SUMMARY_ROW_FORMAT, name);

    bool atLeastOne = false;

    for(size_t i = 0; i < summaryCount; ++i) {
        if(summaries[i].count > 0) {
            if(atLeastOne) {
                printf(", ");
            }

            atLeastOne = true;
            gcladosPrintSummaryCell(summaries[i]);
        }
    }
    printf("\n");
}

void gcladosPrintSummary(size_t totalTestSuites,
                         size_t passedTestSuites,
                         size_t failedTestSuites,
                         size_t totalTests,
                         size_t passedTests,
                         size_t failedTests,
                         GcladosSnapshotStats snapshotSummaryStats,
                         clock_t time) {

    printf("\n");
    GcladosSummary testSuiteSummary[] = {
            {
                    .type = GCLADOS_SUMMARY_FAILURE,
                    .count = failedTestSuites,
                    .prefix = "failed",
            },
            {
                    .type = GCLADOS_SUMMARY_SUCCESS,
                    .count = passedTestSuites,
                    .prefix = "passed",
            },
            {
                    .type = GCLADOS_SUMMARY_NEUTRAL,
                    .count = totalTestSuites,
                    .prefix = "total",
            },
    };

    gcladosPrintSummaryRow("Test suites", testSuiteSummary, sizeof(testSuiteSummary) / sizeof(GcladosSummary));

    GcladosSummary testSummary[] = {
            {
                    .type = GCLADOS_SUMMARY_FAILURE,
                    .count = totalTests - passedTests,
                    .prefix = "failed",
            },
            {
                    .type = GCLADOS_SUMMARY_SUCCESS,
                    .count = passedTests,
                    .prefix = "passed",
            },
            {
                    .type = GCLADOS_SUMMARY_NEUTRAL,
                    .count = totalTests,
                    .prefix = "total",
            },
    };

    gcladosPrintSummaryRow("Tests", testSummary, sizeof(testSummary) / sizeof(GcladosSummary));

    if(snapshotSummaryStats.total > 0) {

        GcladosSummary snapshotSummary[] = {
                {
                        .type = GCLADOS_SUMMARY_FAILURE,
                        .count = snapshotSummaryStats.failed,
                        .prefix = "failed",
                },
                {
                        .type = GCLADOS_SUMMARY_SUCCESS,
                        .count = snapshotSummaryStats.written,
                        .prefix = "written",
                },
                {
                        .type = GCLADOS_SUMMARY_SUCCESS,
                        .count = snapshotSummaryStats.updated,
                        .prefix = "updated",
                },
                {
                        .type = GCLADOS_SUMMARY_SUCCESS,
                        .count = snapshotSummaryStats.passed,
                        .prefix = "passed",
                },
                {
                        .type = GCLADOS_SUMMARY_NEUTRAL,
                        .count = snapshotSummaryStats.total,
                        .prefix = "total",
                },
        };

        gcladosPrintSummaryRow("Snapshots", snapshotSummary, sizeof(snapshotSummary) / sizeof(GcladosSummary));
    }

    printf(GCLADOS_SUMMARY_ROW_FORMAT, "Time");
    gcladosPrintTime(time);

    if(snapshotSummaryStats.total > 0 && snapshotSummaryStats.failed > 0) {
        char buffer[1024];
        sprintf(buffer,
                "%ld snapshot%s failed.",
                snapshotSummaryStats.failed,
                snapshotSummaryStats.failed == 1 ? "" : "s");
        GcladosAnsiFlags flags = gcladosDefaultFailureStyle();
        char *colorizedBuffer = gcladosColors.applyFlags(buffer, flags);
        gcladosColors.freeFlags(flags);

        flags = gcladosColors.createFlags(1, gcladosColors.bold());
        char *command = gcladosColors.applyFlags("`gclados run --updateSnapshot`", flags);
        gcladosColors.freeFlags(flags);

        printf("\n%s. Inspect your code or run %s to update all snapshots.\n", colorizedBuffer, command);

        free(colorizedBuffer);
        free(command);
    }
}

int gcladosRunTestSuites(GcladosTestSuite *suites, size_t count) {
    char buffer[256];
    sprintf(buffer, "Running %lu test suites", count);
    gcladosLog(buffer);

    size_t completedTestSuites = 0;

    GcladosTestSuiteAccumulatedResult *results = calloc(count, sizeof(GcladosTestSuiteAccumulatedResult));

    const clock_t startTime = clock();

    while(completedTestSuites < count) {
        size_t currentSuiteIndex = completedTestSuites;

        sprintf(buffer, "Running %lu/%lu test suite...", currentSuiteIndex + 1, count);
        gcladosLog(buffer);

        results[currentSuiteIndex] = gcladosInitializeTestSuiteResult(suites[currentSuiteIndex]);
        results[currentSuiteIndex].status = GCLADOS_RUNNING;

        bool suiteCompleted = false;

        while(!suiteCompleted) {
            sprintf(buffer,
                    "Running %lu/%lu test from %lu test suite...",
                    results[currentSuiteIndex].completedTestCount + 1,
                    suites[currentSuiteIndex].testCount,
                    currentSuiteIndex + 1);
            gcladosLog(buffer);

            gcladosDrawSuites(suites, results, count, true);
            suiteCompleted = gcladosRunNextTest(suites[currentSuiteIndex], &results[currentSuiteIndex]);

            sprintf(buffer,
                    "Test ended with status: %s",
                    results[currentSuiteIndex].testResults[results[currentSuiteIndex].completedTestCount - 1].pass
                            ? "success"
                            : "failure");
            gcladosLog(buffer);
        }

        gcladosLog("Test suite execution ended.");

        ++completedTestSuites;
        gcladosCompleteTestSuiteResult(&results[currentSuiteIndex]);
    }

    const clock_t endTime = clock();

    gcladosDrawSuites(suites, results, count, false);

    size_t totalTests = 0, passedTests = 0, failedTests = 0;
    size_t totalTestSuites = count, passedTestSuites = 0, failedTestSuites = 0;

    for(size_t i = 0; i < count; ++i) {
        passedTestSuites += results[i].status == GCLADOS_PASS;
        failedTests += results[i].status == GCLADOS_FAILED;
        totalTests += suites[i].testCount;
        for(size_t j = 0; j < results[i].testResultCount; ++j) {
            passedTests += results[i].testResults[j].pass == true;
            failedTests += results[i].testResults[j].pass == false;
        }
    }

    gcladosPrintSummary(totalTestSuites,
                        passedTestSuites,
                        failedTestSuites,
                        totalTests,
                        passedTests,
                        failedTests,
                        gcladosGetSnapshotStats(),
                        endTime - startTime);

    for(size_t i = 0; i < count; ++i) {
        gcladosFreeTestSuiteResult(results[i]);
    }

    return totalTests == passedTests ? EXIT_SUCCESS : EXIT_FAILURE;
}
