#!/usr/bin/env python3

INPUTFILE = "00-original.dat.gz"
OUTPUTFILE = "02-decompressed.dat"

import gzip

def is_gzip(header: bytes) -> bool:
    return header[:2] == b"\x1f\x8b" # gzip magic number

def uses_deflate(header: bytes) -> bool:
    return header[2] == 8 #deflate


with open(INPUTFILE, "rb") as inputfile:
    header = inputfile.read(16)
    assert is_gzip(header)
    assert uses_deflate(header)

data = []
with gzip.open(INPUTFILE, "rb") as inz:
    try:
        while True:
            data.append(inz.read(16))
    except EOFError:
        # we know the stream is truncated.
        pass

with open (OUTPUTFILE, "wb") as outputfile:
        outputfile.write(b"".join(data))