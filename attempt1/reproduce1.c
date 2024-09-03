#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "miniz.h"

#define INP_BLOCKSIZE 0x40000
#define OUT_BLOCKSIZE 0x0400

const char *statustext(int status) {
    switch (status) {
        case MZ_OK:
            return "MZ_OK";
        case MZ_STREAM_END:
            return "MZ_STREAM_END";
        case MZ_NEED_DICT:
            return "MZ_NEED_DICT";
        case MZ_ERRNO:
            return "MZ_ERRNO";
        case MZ_STREAM_ERROR:
            return "MZ_STREAM_ERROR";
        case MZ_DATA_ERROR:
            return "MZ_DATA_ERROR";
        case MZ_MEM_ERROR:
            return "MZ_MEM_ERROR";
        case MZ_BUF_ERROR:
            return "MZ_BUF_ERROR";
        case MZ_VERSION_ERROR:
            return "MZ_VERSION_ERROR";
        case MZ_PARAM_ERROR:
            return "MZ_PARAM_ERROR";
        default:
            return "unknown status";
    }
}

int main(int argc, char *argv[]) {
    unsigned char inp[INP_BLOCKSIZE];
    unsigned char out[OUT_BLOCKSIZE];

    // just making sure I understand the basics:
    assert(sizeof(inp) == INP_BLOCKSIZE * sizeof(unsigned char));
    assert(sizeof(out) == OUT_BLOCKSIZE * sizeof(unsigned char));
    assert(sizeof(unsigned char) == 1);

    // write some not-too-random input data to inp:
    for (int i = 0; i < INP_BLOCKSIZE; i++) {
        int k1 = (13761 * i);
        int k2 = ((i % 3) & 1) + ((i % 7) & 2) + 5 * (k1 >> 8) ^ (i >> 3) ^
                 (k1 << 1);
        inp[i] = 6 * i;
        inp[i] %= 64;
        inp[i] += 64;
        inp[i] ^= (i % 3) & 1;
        inp[i] ^= (i >> 7) ^ (i >> 3) ^ (inp[i] << 1) + (k2 & 0xf1);
    }

    mz_stream stream;
    memset(&stream, 0, sizeof(stream));

    stream.next_in = inp;
    stream.avail_in = INP_BLOCKSIZE;
    stream.next_out = out;
    stream.avail_out = OUT_BLOCKSIZE;

    int status =
        mz_deflateInit2(&stream, MZ_DEFAULT_COMPRESSION, MZ_DEFLATED,
                        -MZ_DEFAULT_WINDOW_BITS, 9, MZ_DEFAULT_STRATEGY);
    if (status != MZ_OK) {
        printf("mz_deflateInit2 failed with status %d (%s)\n", status,
               statustext(status));
        return 1;
    }

    status = mz_deflate(&stream, MZ_NO_FLUSH);
    if (status != MZ_OK) {
        printf("mz_deflate failed with status %d (%s)\n", status,
               statustext(status));
        return 1;
    }

    unsigned int outlen = OUT_BLOCKSIZE - stream.avail_out;
    printf("outlen: %d\n", outlen);

    for (int i = 0; i < outlen; i++) {
        printf("%02x ", out[i]);
    }

    return 0;
}