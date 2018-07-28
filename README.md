# EZPROM #
A small Arduino sketch with functions to read from and write to classic EEPROM chips, and to read from EPROM chips. Great for microprocessor hobbyists looking for low-cost ways to program for their platform.

## How to Use EZPROM ##
To set up Arduino to interface with a parallel memory without modifications to EZPROM pin definitions, connect D2 to the chip enable pin of the memory, D3 to the clock inputs of the shift register(s), D4 to the serial input to the shift register (for multiple series shift registers, connect D4 only to the serial input of the initial register), D5 - D12 to the low 8 address lines of the memory, and D13 to the R / W* pin of the memory. If neccessary, however, pin definitions may be edited in the first lines of the library.

## Layers Upon Layers ##
EZPROM features multiple "layers" of functions that successively add functionality to lower layers. (Here, "lower" means closer to the fundamental protocol.) Of course, the user is free to call functions from any layer but should most often remain at the higher layers. Below is a listing of the various layers and the functions that belong to them. Some functions belonging to the higher layers may still be under construction and thus are not yet available.

### User Layer ###
1. void memHexDump(int na, int startAddress, int numBytes)

Display a neatly formatted hex dump of a block of the memory. Specify the number of address lines being used, the address where the dump should begin, and the number of consecutive bytes that should be read.

2. void memWriteFile(int na, FILE* src)

Write an Intel HEX file to the memory. Specify the number of address lines being used as well as a handle to the HEX file. Note: this function does NOT check the checksums and will not verify the integrity of the HEX file.

### Integrated Layer ###

#### Integrated ByteStream Layer ####
3. void memByteStreamRead(int na, byte* m, int startAddress, int numBytes)

Read a stream of consecutive bytes from the memory. Specify the number of address lines, the pointer to where the read bytes should be stored, the address of the memory where reading should begin, and the number of consecutive bytes that should be read.

4. void memByteStreamWrite(int na, int startAdress, byte* m, int numBytes)

Write a stream of consecutive bytes to the memory. Specify the number of address lines, the address of the memory where writing should begin, the pointer to where the source bytes are, and the number of consecutive bytes that should be written.

#### Integrated Byte Layer ####
5. byte memByteRead(int na, int add)

Read a single byte from memory at a specific address. Specify the number of address lines as well as the read address.

6. byte memByteWrite(int na, int add, byte data)

Write a single byte to memory at a specific address. Specify the number of address lines, the write address, and the byte that will be written.

### Parallel Layer ###

#### Parallel Byte Layer ####
7. byte readByte()

Reads a byte from the parallel bus.

8. void writeByte(byte b)

Writes a byte to the parallel bus.

#### Parallel SubByte Layer ####
9. void setPortsAs(bool in)

Sets the parallel lines as input or output. Call will true to set as input or false to set as output.

### Serial Layer ###

#### Serial ByteStream Layer ####
10. void sendAddress(int na, int add)

Write an address to the shift registers. Specify the number of address lines and the address that will appear on the shift registers.

11. void binaryCtr(int d)

Tests the serial bus by running a binary counter on the shift registers. Specify the delay between each update.

12. void sendByteStream(byte* b, int numBytes)

Sends a stream of bytes to the shift registers, with least significant byte sent first. Specify the pointer to the bytes and the number to be written (generally, numBytes == sizeof(b)).

#### Serial Byte Layer ####
13. void sendByte(byte b)

Sends exactly one byte to the shift registers.

#### Serial SubByte Layer ####
14. void pulseClock(int d)

Pulse the serial clock, shifting the shift registers. Specify the delay for the shift (for most cases, a delay of 0 is desired).

## Troubleshooting ##
If the hexdump produces bytes that are consistent across a large block but are random, it is likely that the memory is not actually connected and thus data pins are floating on the read operation. Use an Ohmmeter to check all connections.

If writing has no effect on the memory's readout, check that the R / W* signal is properly connected. If so, confirm that the chip is actually an EEPROM (EPROMs can be written to but not with this setup).
