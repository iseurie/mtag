#include "fingerprinter.h"

int main (int argc, char** argv) {
    ftarget_idx_t targets = fetch_targets_or_panic(argc, argv);
    for(int i = 0; i < targets.namec; ++i) {
        fingerprint char[256];
    }
    free_ftargets(&targets);
}