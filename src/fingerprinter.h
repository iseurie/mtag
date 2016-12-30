#ifndef H_FINGERPRINTER
#define H_FINGERPRINTER
#define AUDIO_INBUF_SIZE 20480
#define MAX_AUDIO_FRAME_SIZE 192000

#include "targets.h"
#include <chromaprint.h>
#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>

#define ERR_FP_FOPEN_FAILED -1

typedef struct fp_s {
    int fpc;
    uint32_t** fpv;
} fp_t;

void fp_free(fp_t* p);
char fp_raw_isFull(fp_t* p);
typedef MTFPCtx struct {
    enum ChromaprintAlgorithm;
    FILE* fp_input;
    AVCodec* p_codec;
    AVCodecContext* p_cdcCtx;
    size_t input_len;
    size_t seg_len; // length of a single fingerprint in frames
    fp_t payload;
}

int fp_ctx_start(size_t segment_len, char* ifname, MTFPCtx* ctx);
int fp_ctx_close(MTFPCtx* ctx);
void fp_ctx_free(MTFPCtx* ctx);
int fp_ctx_encode(char** fpv, const MTFPCtx* ctx);

// calculate a fingerprint, using the given count of fingerprints
// return count of fingerprints calculated; return <0 indicates failure
int fp_calc_ratio(double ratio, const char* ifname, MTFPCtx* ctx);
int fp_calc_count(int count, const char* ifname, MTFPCtx* ctx);
#endif