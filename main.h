#ifndef _main_h_
#define _main_h_

#ifdef __cplusplus
extern "C" {
#endif

const unsigned char GZIP_HEADER[] = {
    0x1f, 0x8b,              // gzip magic number
    0x08,                    // compression method = DEFLATE
    0x00,                    // flags, nothing set
    0x00, 0x00, 0x00, 0x00,  // mtime
    0x00,                    // extra flags
    0x00                     // OS (unknown/Windows)
};

void sicherlich(int pred, const char *msg, const char *sourcefile, int line) {
    if (!pred) {
        fprintf(stderr, "Error in %s:%d: sicher nicht: %s\n", sourcefile, line,
                msg);
        exit(1);
    }
}

#define sicher(pred) (sicherlich(!!(pred), #pred, __FILE__, __LINE__))
// #define sicher(pred) ((void)0)

size_t filesize(const char *path) {
    FILE *f = fopen(path, "rb");
    if (!f) {
        fprintf(stderr, "Error: fopen() failed for path %s\n", path);
        exit(1);
    }
    fseek(f, 0, SEEK_END);
    size_t size = ftell(f);
    fclose(f);
    return size;
}

#ifdef __cplusplus
}
#endif

#endif