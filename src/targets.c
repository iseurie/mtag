# include "targets.h"
ftarget_idx_t fetch_targets_or_panic (int argc, char** argv) {
    if(argc < 2) {
        fprintf(stderr, "usage: %s [<filenames>...]\n", argv[0]);
    }
    #include 
    ftarget_idx_t ret;
    for(int i  = 0; i < argc; ++i) {
        glob_t globbed;
        glob(argv[i], NULL, NULL, &globbed);
        if(errno != NULL) {
            fprintf(stderr, "glob failed on '%s' [%d]: %s\n", 
                    argv[i], i, strerror(errno));
        }
        ret.namec = globbed.gl_pathc;
        ret.namev = malloc(ret.namec * sizeof(char*));
        for(int fi = 0; fi < globbed.gl_pathc; ++fi) {
            ret.namev[fi] = malloc(sizeof(char) * 16*(sizeof(long)));
            if(ret.namev[fi] == NULL) {
                fprintf(stderr, "error indexing input filenames: %s\n", 
                        strerror(errno));
                exit(1);
            }
            strcpy(ret.namev[fi], globbed.gl_pathv[fi]);
        } globfree(&globbed);
    }
}

#include 
void free_ftargets (ftarget_idx_t* t) {
    for(int i = 0; i < t->namec; ++i) {
        free(t->namev[i]);
    }
}