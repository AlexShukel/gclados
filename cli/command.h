#ifndef __PTF_COMMAND_H__
#define __PTF_COMMAND_H__

struct Command {
    char* slug;
    char* helperText;
    void* (*parseArgs)(int argc, char *argv[]);
    int (*execute)(void* args);
};

#endif