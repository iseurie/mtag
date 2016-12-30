#include "fingerprinter.h"

void fp_free(fp_t* p) {
    for(int i = 0; i < p.fpc; ++i) {
        free(p.fpv[i]);
        free(p.fpv);
    }
}

char fp_raw_isFull (uint32_t* p) {
    *p | 0x01 << 32 ? return 1 : return NULL;
}

int fp_calc_ratio(double ratio, const char* ifname, MTFPCtx* ctx) {
    fp_ctx_close(ctx);
    size_t seg_len = (0x01 << 32 / MAX_AUDIO_FRAME_SIZE) * 0.8;
    int r = fp_ctx_start(seg_len, ifname, ctx);
    if(r < 0) return r;
    size_t samples_len = ctx->input_len * ratio;
    size_t samplec = ctx->input_len / samples_len;
    char skip = 1;
    while(p_frame->pkt_size > 0) {
        if(skip != NULL) {
            r = fseek(ctx->fp_input, samples_len * (1.0 / ratio), SEEK_SET);
            if(r != 0) return r;
            skip = NULL;
        } else seek = 1;
        counter = 0;
    }
}

int calc_fingerprint_base64(char* fp, const char* target) {
    int r;
    AVFrame* p_frame;
    AVCodecContext* p_cdcCtx;
    AVCodec* p_codec;
    AVPacket avpkt;
    AVFormatContext* p_fmtCtx = avformat_alloc_context();
    FILE* f_target = fopen(target, "rb");
    if(!f_target) {
        fprintf(stderr, "fingerprint: fopen() failed: %d (%s)\n", errno, strerror(errno));
        return r;
    }
    uint8_t inbuf[AUDIO_INBUF_SIZE + FF_INPUT_BUFFER_PADDING_SIZE];
    int16_t* outbuf = malloc(MAX_AUDIO_FRAME_SIZE);
    if(r = avformat_open_input(&p_fmtCtx, target, 0, NULL) != 0) {
        fprintf(stderr, "fingerprint: ffmpeg: avformat_open_input() failed: %d\n", r);
        return r;
    }
    if(r = avformat_find_stream_info(p_fmtCtx, NULL) < 0) {
        fprintf(stderr, "fingerprint: ffmpeg: avformat_find_stream_info() failed: %d\n", r);
        return r;
    }
    p_codec = avcodec_alloc_context3(NULL);
    av_find_best_stream(
            p_fmtCtx, AVMEDIA_TYPE_AUDIO, 1, -1, &p_codec, NULL);
    if(!p_codec) {
        fprintf(stderr, "fingerprint: ffmpeg: cannot find codec\n");
        return r;
    }
    avformat_close_input(p_fmtCtx);
    avformat_free_context(p_fmtCtx);
    if(r = avcodec_open2(p_cdcCtx, p_codec, NULL) < 0) {
        fprintf(stderr, "fingerprint: ffmpeg: cannot open codec: %d\n");
        return r;
    }
    avpkt.data = inbuf;
    avpkt.size = fread(inbuf, 1, AUDIO_INBUF_SIZE, f_target);
    ChromaprintContext* crCtx = chromaprint_new(CHROMAPRINT_ALGORITHM_DEFAULT);
    if(r = chromaprint_start(
        crCtx, p_cdcCtx->sample_rate, p_cdcCtx->channels) < 1) {
        fprintf(stderr, "fingerprint: ChromaPrint: cannot open audio (%d)\n", r);
        return r;
    }
    p_frame = av_frame_alloc();
    while(p_frame->pkt_size > 0) {
        int* p_got_frame;
        size_t len = avcodec_decode_audio4(
                p_cdcCtx, p_frame, p_got_frame, &avpkt);
        if(len < 0) {
            fprintf(stderr, "fingerprint: ffmpeg: cannot decode audio (%d)\n", len);
            return len;
        }
        if(p_got_frame) {
            r = chromaprint_feed(crCtx, (int16_t*)&p_frame->data, p_frame->pkt_size);
            if(r < 1) {
                fprintf(stderr, "fingerprint '%s': ChromaPrint: cannot process audio (%d)\n", r);
                return r;
            }
        }
    }
    av_frame_free(p_frame);
    if(r = chromaprint_finish(crCtx) < 1) {
        fprintf(stderr, "fingerprint: ChromaPrint: cannot flush audio buffer (%d)\n", r);
        return r;
    }
    fclose(f_target);
    avcodec_free_context(&p_cdcCtx);
    avformat_free_context(p_fmtCtx);
    return 0;
}