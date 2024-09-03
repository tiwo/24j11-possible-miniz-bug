    ⚠️ this is unfinished business. There is nothing to see here.

# possible miniz bug

WHile compressing some highly compressible data with miniz, I've come across a data error that I can't exmplain: One 16-bit word seems to have doubled in the middle of the stream.

I know the most likely issue is me doing something wrong while looping over the data and iterating over `mz_deflate(&outstream, MZ_NO_FLUSH)` calls. Yet I don't see the issue.

So I'm trying to reproduce this error here in the hopes of either finding a miniz bug, or learning something valuable.

## Overview

This repository starts with the errorneous artifact (a GZIP file produced with miniz), `00-original.dat.gz`

Data files are numbered with even numbers, processing is done by programs with odd numbers.

From `00-original.dat.gz`, the script `01-decompress-with-gzip.py` produces `02-decompressed.dat`, etc.

The directory `miniz` contains miniz 3.0.2.

## The data error

Look at this section of my highly compressible data. What you see was compressed with C calls to miniz, and then uncompressed with gzip. The first column is the address of each 16-byte block, followed by 8 columns containing those bytes. I'm using dashes for the hexadecimal zero, to make the patterns more obvious:

    ---b9aa-: -f-- ---- 18-d ---- ba-4 ---- ---- ----
    ---b9ab-: 1--- ---- 18-d ---- ba-4 ---- ---- ----
    ---b9ac-: 1--- ---- 18-d ---- ba-4 ---- ---- ----
    ---b9ad-: -f-- ---- 18-d ---- ba-4 ---- ---- ----
    ---b9ae-: 1--- ---- 18-d ---- ba-4 ---- ---- ----
    ---b9af-: 1--- ---- 18-d ---- ba-4 ---- ---- ----
    ---b9b--: 1--- ---- 18-d ---- ba-4 ---- ---- ----
    ---b9b1-: 1--- ---- 18-d ---- ba-4 ---- ---- ----
    ---b9b2-: -f-- ---- 18-d ---- ba-4 ---- ---- ----
    ---b9b3-: 1--- ---- 18-d ---- ba-4 ---- ---- ----
    ---b9b4-: 1--- 1--- ---- 1a-e ---- -e-3 ---- ----
    ---b9b5-: ---- 1--- ---- 1a-e ---- -e-3 ---- ----
    ---b9b6-: ---- 1--- ---- 1a-e ---- -e-3 ---- ----
    ---b9b7-: ---- -f-- ---- 1a-e ---- -e-3 ---- ----
    ---b9b8-: ---- 1--- ---- 1a-e ---- -e-3 ---- ----
    ---b9b9-: ---- -f-- ---- 1a-e ---- -e-3 ---- ----
    ---b9ba-: ---- 1--- ---- 1a-e ---- -e-3 ---- ----
    ---b9bb-: ---- 1--- ---- 1a-e ---- -e-3 ---- ----
    ---b9bc-: ---- 1--- ---- 1a-e ---- -e-3 ---- ----
    ---b9bd-: ---- 1--- ---- 1a-e ---- -e-3 ---- ----
    ---b9be-: ---- -f-- ---- 1a-e ---- -e-3 ---- ----
    ---b9bf-: ---- 1--- ---- 1a-e ---- -e-3 ---- ----
    ---b9c--: ---- -f-- ---- 1a-e ---- -e-3 ---- ----

The data comes in blocks four little-endian 32-bit integers:

- The first integer is really close to 16. In the sample file, it's either 0x10 or 0x0f (milliseconds - it's the time difference between two consecutive records) - except the first one in the file which is has any value between 0 and 999 with equal probability.

- The next two 32-bit integers are mouse coordinates.

- the last integer is always 0.

Look what happened at `---b942`; the 0x10 timestamp seems to have duplicated. Or maybe the "jump" in mouse coordinates - from 0x04ba to 0x03e0, that's a jump of 218 pixels in the y direction - indicate that something more interesting might have happened with the C program.

Whatever the cause, I'll try and reproduce the compression.
