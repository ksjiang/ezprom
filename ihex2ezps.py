# -*- coding: utf-8 -*-
"""
Intel HEX to EZProm DataStream Converter
To convert assembler outputs to a form usable
  by EZProm's main library
Created on Sun Mar  4 13:35:34 2018

@author: Kyle
"""

#converts Intel HEX to EZDS
def convert(ihexname, lines, base):
    lines, base = int(lines), int(base)
    
    if (lines < 1) or (lines > 16):
        raise ValueError("Wrong number of lines. Must be between 1 and 16.")
        
    if (base < 0) or (base > (2 ** 16) - (2 ** lines)) or (base % (2 ** lines)):
        raise ValueError("Invalid base address.")
        
    if not(ihexname[-4: ] == ".hex"):
        raise IOError("Wrong Intel HEX file extension. Expected: .hex, got: " + ihexname[-4: ])
        
    source = open(ihexname, 'r')
    dest = open(ihexname[: -4] + '.ezds', 'w+')
    #annoying off-by-one error
    writeTo = float('NaN')
    firstLine = True
    
    for line in source:
        contents = bytearray.fromhex(line[1: -1])
        
        if contents[3]:
            break
        
        dataSize = contents[0]
        address = 256 * contents[1] + contents[2]
        offset = address - base
        
        #range is completely out of programmable segment
        if (offset > 2 ** lines - 1) or (offset + dataSize < 0):
            continue        #to next line
        
        #verify checksum     
        if sum(contents) % 256:
            dest.close()
            raise ValueError("Checksum invalid. Aborting rest of write...")
            
        #account for jumps
        if not (writeTo + 1 == max(base, address)):
            s = ':' + hex(max(base, address))[2: ].zfill(4)
            
            if not firstLine:
                s = '\n' + s
            
            firstLine = False
            dest.write(s)
        
        for i in range(dataSize):
            writeTo = address + i
            if (writeTo >= base) and (writeTo < base + 2 ** lines):
                dest.write(hex(contents[i + 4])[2: ].zfill(2).upper())
        
    source.close()
    dest.close()
    
    return