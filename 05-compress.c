#include <stdio.h>
#include <stdlib.h>

#include "main.h"
#include "miniz/miniz.h"

const char *INPUTFILE = "04-without-glitch.dat";
const char *OUTPUTFILE = "06-compressed.dat";

// we read and compress blocks of 16*1024==16348 bytes of uncompressed data
#define BLOCKSIZE 0x4000

// we write compressed data in blocks of 2048 bytes. This is the maximum we'll
// ever get from miniz at once. I think this is allowed.
#define COMPRESSED_BUFFER_SIZE 0x0400

int main() {
    unsigned char uncompressed[BLOCKSIZE];
    unsigned char compressed[COMPRESSED_BUFFER_SIZE];

    sicher(BLOCKSIZE == sizeof(uncompressed));
    sicher(BLOCKSIZE == 16 * 1024);
    sicher(BLOCKSIZE == 16384);

    sicher(COMPRESSED_BUFFER_SIZE == sizeof(compressed));
    // sicher(COMPRESSED_BUFFER_SIZE == 2048);

    sicher(filesize(INPUTFILE) % BLOCKSIZE == 0);

    FILE *inpf = fopen(INPUTFILE, "rb");
    sicher(inpf);

    FILE *outf = fopen(OUTPUTFILE, "wb");
    sicher(outf);

    // write GZIP header to outf:
    size_t written = fwrite(GZIP_HEADER, 1, sizeof(GZIP_HEADER), outf);
    sicher(written == sizeof(GZIP_HEADER));

    // initialze miniz compressor:
    mz_stream compressor;
    memset(&compressor, 0, sizeof(compressor));
    int status =
        mz_deflateInit2(&compressor, MZ_DEFAULT_COMPRESSION, MZ_DEFLATED,
                        -MZ_DEFAULT_WINDOW_BITS, 9, MZ_DEFAULT_STRATEGY);
    sicher(status == MZ_OK);

    long bytes_written = 0;
    long blocks_read = 0;
    size_t outlen;

    while (1) {
        printf("blocks_read: %ld\n", blocks_read);
        // read uncompressed data
        size_t uncompressed_size = fread(uncompressed, 1, BLOCKSIZE, stdin);
        if (uncompressed_size == 0) {  // EOF
            break;
        }

        if (uncompressed_size != BLOCKSIZE) {
            fprintf(stderr, "Warning: read %d > %zd bytes\n", BLOCKSIZE,
                    uncompressed_size);
        }

        // no partial blocks in the file:
        sicher(uncompressed_size == BLOCKSIZE);

        blocks_read++;

        printf("uncompressed_size: %zu\n", uncompressed_size);
        // feed the block to the coompressor:
        compressor.next_in = (unsigned char *)uncompressed;
        compressor.avail_in = BLOCKSIZE;
        compressor.next_out = (unsigned char *)compressed;
        compressor.avail_out = COMPRESSED_BUFFER_SIZE;

        do {
            status = mz_deflate(&compressor, MZ_NO_FLUSH);
            printf("status: %d\n", status);
            sicher(status != MZ_STREAM_ERROR);  // stream is bogus
            sicher(status != MZ_PARAM_ERROR);   // parameters are bogus
            sicher(status != MZ_BUF_ERROR);  // no forward progress is possible
            sicher(status == MZ_OK);

            outlen = COMPRESSED_BUFFER_SIZE - compressor.avail_out;
            fwrite(compressed, 1, outlen, outf);
            bytes_written += outlen;
        } while (outlen > 0);
    }

    do {
        status = mz_deflate(&compressor, MZ_FINISH);
        sicher(status != MZ_STREAM_ERROR);  // stream is bogus
        sicher(status != MZ_PARAM_ERROR);   // parameters are bogus
        sicher(status != MZ_BUF_ERROR);     // no forward progress is possible
        sicher(status == MZ_OK || status == MZ_STREAM_END);
        outlen = COMPRESSED_BUFFER_SIZE - compressor.avail_out;
        fwrite(compressed, 1, outlen, outf);
        bytes_written += outlen;
    } while (status != MZ_STREAM_END);

    status = mz_deflateEnd(&compressor);
    sicher(status == MZ_OK);

    fclose(inpf);
    fclose(outf);

    fprintf(stderr, "Compressed %zu bytes to %zu bytes\n", filesize(INPUTFILE),
            bytes_written + sizeof(GZIP_HEADER));

    return 0;
}