#include "fingerprinter.h"

int fp_ctx_start(size_t segment_len, char* ifname, MTFPCtx* ctx) {
    FILE* fp = fopen(ifname, "rb");
    if(!fp) return ERR_FP_FOPEN_FAILED;
    fseek(fp, 0, SEEK_END);
    ctx->input_len = ftell(fp);
    rewind(fp);
    ctx->seg_len = segment_len;
    ctx->payload.fpc = 0;
    ctx->fp_input = fp;
}

int fp_ctx_close(MTFPCtx* ctx) {
    int r = fclose(ctx->fp_input);
    if(r != 0) return r;
}

void fp_ctx_free(MTFPCtx* ctx) {
    fp_free(ctx->payload);
}

int fp_ctx_encode(char** fpv, const MTFPCtx* ctx) {
    for(int i = 0; i < ctx->payload.fpc; ++i) {
        char** out;
        int r = chromaprint_encode_fingerprint(ctx->payload[i], 1, 1, out);
        r == 1 ? return 0 : return -r;
        fpv[i] = out[0];
    }
}