#include "snapshots.h"
#include "fileUtils.h"
#include "ioutils.h"

#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#define MAX_IDENTIFIER_LENGTH 1024

typedef struct {
    void *value;
    size_t size;
} GcladosSnapshot;

bool GCLADOS_UPDATE_SNAPSHOTS = false;

size_t GCLADOS_SNAPSHOT_INDEX_COUNTER = 0;

void gcladosResetSnapshotCounter() {
    GCLADOS_SNAPSHOT_INDEX_COUNTER = 0;
}

void gcladosSetUpdateSnapshots(bool updateSnapshots) {
    GCLADOS_UPDATE_SNAPSHOTS = updateSnapshots;
}

GcladosSnapshotStats gcladosSnapshotStats = {
        .total = 0,
        .failed = 0,
        .updated = 0,
        .written = 0,
        .passed = 0,
};

GcladosSnapshotStats gcladosGetSnapshotStats() {
    return gcladosSnapshotStats;
}

bool gcladosMoveCursorToSnapshot(FILE *snapshotFile, const char searchIdentifier[]) {
    size_t snapshotCount;
    fread(&snapshotCount, sizeof(size_t), 1, snapshotFile);

    for(size_t i = 0; i < snapshotCount; ++i) {
        char identifier[MAX_IDENTIFIER_LENGTH];
        size_t size;

        fread(identifier, sizeof(char), MAX_IDENTIFIER_LENGTH, snapshotFile);
        fread(&size, sizeof(size_t), 1, snapshotFile);

        if(strcmp(identifier, searchIdentifier) == 0) {
            fseek(snapshotFile, -((long) sizeof(size_t)), SEEK_CUR);
            return true;
        } else {
            fseek(snapshotFile, (long) size, SEEK_CUR);
        }
    }

    return false;
}

size_t gcladosGetNumberLength(size_t number) {
    if(number == 0) {
        return 1;
    }

    return ((size_t) log10((double) number)) + 1;
}

void gcladosConstructIdentifierName(const char functionName[], size_t id, char identifier[MAX_IDENTIFIER_LENGTH]) {
    if(strlen(functionName) + gcladosGetNumberLength(id) > MAX_IDENTIFIER_LENGTH - 1) {
        gcladosPanic("Snapshot identifier exceeded limit. Please, shorten test name.", EXIT_FAILURE);
    }

    sprintf(identifier, "%s(%lu)", functionName, id);
}

void gcladosGetSnapshotFilename(const char fileName[], char snapshotFilename[FILENAME_MAX]) {
    strcpy(snapshotFilename, fileName);
    strcat(snapshotFilename, ".snap");
}

FILE *gcladosOpenSnapshotFile(const char fileName[], const char modes[]) {
    char snapshotFilename[FILENAME_MAX];

    gcladosGetSnapshotFilename(fileName, snapshotFilename);

    return fopen(snapshotFilename, modes);
}

GcladosSnapshot *gcladosGetSnapshot(StatementContext context, size_t snapshotIndex) {
    FILE *snapshotFile = gcladosOpenSnapshotFile(context.fileName, "rb");

    if(snapshotFile == NULL) {
        return NULL;
    }

    char identifier[FILENAME_MAX];
    gcladosConstructIdentifierName(context.functionName, snapshotIndex, identifier);

    if(gcladosMoveCursorToSnapshot(snapshotFile, identifier)) {
        GcladosSnapshot *snapshot = malloc(sizeof(GcladosSnapshot));
        fread(&snapshot->size, sizeof(size_t), 1, snapshotFile);
        snapshot->value = malloc(sizeof(snapshot->size));
        fread(snapshot->value, sizeof(char), snapshot->size, snapshotFile);

        fclose(snapshotFile);
        return snapshot;
    }

    fclose(snapshotFile);
    return NULL;
}

void gcladosSaveNewSnapshot(StatementContext context, void *value, size_t size, size_t snapshotIndex) {
    gcladosSnapshotStats.written += 1;

    FILE *snapshotFile = gcladosOpenSnapshotFile(context.fileName, "rb+");

    if(snapshotFile == NULL) {

        char snapshotFileName[FILENAME_MAX];
        gcladosGetSnapshotFilename(context.fileName, snapshotFileName);
        gcladosEnsureFileExists(snapshotFileName);

        snapshotFile = fopen(snapshotFileName, "rb+");
    }

    size_t snapshotCount = 0;
    if(fread(&snapshotCount, sizeof(size_t), 1, snapshotFile) != 1) {
        snapshotCount = 0;
    }

    snapshotCount += 1;
    fseek(snapshotFile, 0, SEEK_SET);
    fwrite(&snapshotCount, sizeof(size_t), 1, snapshotFile);

    for(size_t i = 0; i < snapshotCount - 1; ++i) {
        fseek(snapshotFile, MAX_IDENTIFIER_LENGTH * sizeof(char), SEEK_CUR);
        size_t blockSize;
        fread(&blockSize, sizeof(size_t), 1, snapshotFile);
        fseek(snapshotFile, (long) blockSize, SEEK_CUR);
    }

    char identifier[MAX_IDENTIFIER_LENGTH];
    memset(identifier, 0, MAX_IDENTIFIER_LENGTH);
    gcladosConstructIdentifierName(context.functionName, snapshotIndex, identifier);

    fwrite(identifier, sizeof(char), MAX_IDENTIFIER_LENGTH, snapshotFile);
    fwrite(&size, sizeof(size_t), 1, snapshotFile);
    fwrite(value, sizeof(char), size, snapshotFile);

    fclose(snapshotFile);
}

void updateSnapshot(StatementContext context, void *value, size_t size, size_t snapshotIndex) {
    gcladosSnapshotStats.updated += 1;

    FILE *snapshotFile = gcladosOpenSnapshotFile(context.fileName, "rb+");

    if(snapshotFile == NULL) {
        gcladosPanic("Could not update snapshot because snapshot file does not exist.", ENOENT);
    }

    char identifier[FILENAME_MAX];
    gcladosConstructIdentifierName(context.functionName, snapshotIndex, identifier);

    if(gcladosMoveCursorToSnapshot(snapshotFile, identifier)) {
        size_t oldSize;
        fread(&oldSize, sizeof(size_t), 1, snapshotFile);
        fseek(snapshotFile, -((long) sizeof(size_t)), SEEK_CUR);
        fwrite(&size, sizeof(size_t), 1, snapshotFile);
        long cursorPosition = ftell(snapshotFile);

        gcladosMoveBlock(snapshotFile, ((long) size) - ((long) oldSize));
        fseek(snapshotFile, cursorPosition, SEEK_SET);
        fwrite(value, sizeof(char), size, snapshotFile);
    } else {
        fclose(snapshotFile);
        gcladosPanic("Could not update snapshot: snapshot does not exist in file.", EXIT_FAILURE);
    }

    fclose(snapshotFile);
}

typedef struct {
    size_t size;
    GcladosSnapshot *snapshot;
} GcladosSnapshotPredicateOptions;

bool gcladosSnapshotsEqual(GcladosSnapshot first, GcladosSnapshot second) {
    if(first.size != second.size) {
        return false;
    }

    return memcmp(first.value, second.value, first.size) == 0;
}

bool gcladosToMatchSnapshotPredicate(StatementContext context, void **value, GcladosSnapshotPredicateOptions *options) {
    gcladosSnapshotStats.total += 1;
    ++GCLADOS_SNAPSHOT_INDEX_COUNTER;

    GcladosSnapshot *snapshot = gcladosGetSnapshot(context, GCLADOS_SNAPSHOT_INDEX_COUNTER);


    if(snapshot == NULL) {
        gcladosSaveNewSnapshot(context, *value, options->size, GCLADOS_SNAPSHOT_INDEX_COUNTER);

        return true;
    }

    options->snapshot = snapshot;

    bool result = gcladosSnapshotsEqual(
            (GcladosSnapshot){
                    .size = options->size,
                    .value = *value,
            },
            *snapshot);

    if(!result && GCLADOS_UPDATE_SNAPSHOTS) {
        updateSnapshot(context, *value, options->size, GCLADOS_SNAPSHOT_INDEX_COUNTER);
        return true;
    }

    if(!result) {
        gcladosSnapshotStats.failed += 1;
    } else {
        gcladosSnapshotStats.passed += 1;
    }

    return result;
}

// Function which converts snapshot to string
char *gcladosToMatchSnapshotExpectedValue(void **value, const GcladosSnapshotPredicateOptions *options, bool pass) {
    if(!options->snapshot) {
        gcladosPanic("Could not print snapshot value because snapshot does not exist", EXIT_FAILURE);
    }

    char *valueAsHex = gcladosConvertToHex(options->snapshot->value, options->snapshot->size);

    if(pass) {
        char *valueWithPrefix = calloc(sizeof(char), strlen(valueAsHex) + 4);
        sprintf(valueWithPrefix, "not %s", valueAsHex);
        free(valueAsHex);

        return valueWithPrefix;
    } else {
        return valueAsHex;
    }
}

// Function for converting received value to hex string.
char *gcladosToMatchSnapshotReceivedValue(void **value, const GcladosSnapshotPredicateOptions *options, bool pass) {
    if(pass) {
        return NULL;
    } else {
        return gcladosConvertToHex(*value, options->size);
    }
}

GcladosPredicate gcladosToMatchSnapshot(size_t size) {
    GcladosSnapshotPredicateOptions *options = malloc(sizeof(GcladosSnapshotPredicateOptions));
    *options = (GcladosSnapshotPredicateOptions){
            .size = size,
            .snapshot = NULL,
    };

    GcladosPredicate predicate = {
            .options = options,
            .usage = "gclados.toMatchSnapshot(size)",
            .execute = (bool(*)(StatementContext, void *, void *)) gcladosToMatchSnapshotPredicate,
            .expectedValueToString = (GcladosValueToStringConverter) gcladosToMatchSnapshotExpectedValue,
            .receivedValueToString = (GcladosValueToStringConverter) gcladosToMatchSnapshotReceivedValue,
            .customOutput = false,
            .free = NULL,
    };

    return predicate;
}
