#ifndef H_FTARGETINDEX
#define H_FTARGETINDEX
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <glob.h>
typedef struct ftarget_idx_s {
    char** namev;
    int namec;
} ftarget_idx_t;

ftarget_idx_t fetch_targets_or_panic(int argc, char** argv);
void free_targets(ftarget_idx_t* t);
#endif