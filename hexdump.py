#!/usr/bin/env python3
"""
- Accept one argument on the command line: the input file name
- print a hex dump of the bytes from position 0xb9aa0 to 0xb9c0f.
"""

START, END = 0xb9aa0, 0xb9c0f
BLOCKSIZE = 16

# For illustration, here's the output of `hexdump.py 02-without-error.dat `:
#
#    000b9aa0: -f -- -- -- 18 -d -- -- ba -4 -- -- -- -- -- --
#    000b9ab0: 1- -- -- -- 18 -d -- -- ba -4 -- -- -- -- -- --
#    000b9ac0: 1- -- -- -- 18 -d -- -- ba -4 -- -- -- -- -- --
#    000b9ad0: -f -- -- -- 18 -d -- -- ba -4 -- -- -- -- -- --
#    000b9ae0: 1- -- -- -- 18 -d -- -- ba -4 -- -- -- -- -- --
#    000b9af0: 1- -- -- -- 18 -d -- -- ba -4 -- -- -- -- -- --
#    000b9b00: 1- -- -- -- 18 -d -- -- ba -4 -- -- -- -- -- --
#    000b9b10: 1- -- -- -- 18 -d -- -- ba -4 -- -- -- -- -- --
#    000b9b20: -f -- -- -- 18 -d -- -- ba -4 -- -- -- -- -- --
#    000b9b30: 1- -- -- -- 18 -d -- -- ba -4 -- -- -- -- -- --
#    000b9b40: 1- -- -- -- 1a -e -- -- -e -3 -- -- -- -- -- --
#    000b9b50: 1- -- -- -- 1a -e -- -- -e -3 -- -- -- -- -- --
#    000b9b60: 1- -- -- -- 1a -e -- -- -e -3 -- -- -- -- -- --
#    000b9b70: -f -- -- -- 1a -e -- -- -e -3 -- -- -- -- -- --
#    000b9b80: 1- -- -- -- 1a -e -- -- -e -3 -- -- -- -- -- --
#    000b9b90: -f -- -- -- 1a -e -- -- -e -3 -- -- -- -- -- --
#    000b9ba0: 1- -- -- -- 1a -e -- -- -e -3 -- -- -- -- -- --
#    000b9bb0: 1- -- -- -- 1a -e -- -- -e -3 -- -- -- -- -- --
#    000b9bc0: 1- -- -- -- 1a -e -- -- -e -3 -- -- -- -- -- --
#    000b9bd0: 1- -- -- -- 1a -e -- -- -e -3 -- -- -- -- -- --
#    000b9be0: -f -- -- -- 1a -e -- -- -e -3 -- -- -- -- -- --
#    000b9bf0: 1- -- -- -- 1a -e -- -- -e -3 -- -- -- -- -- --
#    000b9c00: -f -- -- -- 1a -e -- -- -e -3 -- -- -- -- -- --

import sys

if len(sys.argv) != 2:
    print(f"Usage: {sys.argv[0]} <filename>")
    sys.exit(1)

with open(sys.argv[1], "rb") as inputfile:
    data = inputfile.read()
    assert(isinstance(data, bytes))

if len(data) < END + BLOCKSIZE:
    print(f"File is too short, it has only {len(data)} == {hex(len(data))} bytes.")
    sys.exit(1)

for i in range(START, END, BLOCKSIZE):
    position = f"    {i:08x}: "
    hexdump = []
    for j in range(BLOCKSIZE):
        hexdump.append(f"{data[i + j]:02x}")
    hexdump = " ".join(hexdump).replace("0", "-")
    print(position + hexdump)
