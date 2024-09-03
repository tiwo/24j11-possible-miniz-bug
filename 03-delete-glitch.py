#!/usr/bin/env python3
"""
- Read from 01-with-error.dat
- Remove two bytes at positions 0xb9b4 and 0xb9b5.
- Also, truncate the data end at some 1024-byte boundary well after the error.
- Write to 02-without-error.dat
"""

INPUTFILE = "02-decompressed.dat"
OUTPUTFILE = "04-without-glitch.dat"
REMOVE = (0xb9b40, 2)
CUTOFF =  0x200000    # 2 MiB

#Length of data before removing error: 3010720 == 0x2df0a0
#Length of data after removing error:  3010726 == 0x2df0a6
#Length of data after cutting off:     2097152 == 0x200000

with open(INPUTFILE, "rb") as inputfile:
    data = inputfile.read()

print(f"Length of data before removing error: {len(data)} == {hex(len(data))}")

offset, length = REMOVE
data = data[:offset] + data[offset + length:]

print(f"Length of data after removing error:  {len(data)} == {hex(len(data))}")

data = data[:CUTOFF]

print(f"Length of data after cutting off:     {len(data)} == {hex(len(data))}")

with open(OUTPUTFILE, "wb") as outputfile:
    outputfile.write(data)

