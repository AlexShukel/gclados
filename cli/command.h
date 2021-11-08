#ifndef __PTF_COMMAND__
#define __PTF_COMMAND__

struct Command {
    char* slug;
    char* helperText;
    void* (*parseArgs)(int argc, char *argv[]);
    int (*execute)(void* args);
};

#endif