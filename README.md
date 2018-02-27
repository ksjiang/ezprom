# ezprom #
An Arduino library to read from and write to classic EEPROM chips, and to read from EPROM chips. Great for microprocessor hobbyists looking for low-cost ways to program for their platform.

## How to Use EZPROM ##
The EZPROM library takes advantage of a serial-type data transmission using shift registers from two pins selected on the Arduino. Default, based on precendences set in [BB85](https://github.com/siyujiang81/bb85), is to let Pin 4 be the Serial shift Data and Pin 3 be the Clock line. Further, Pin 2 is defined as the latch (after address is stable), and Pin 1 is the Read / Write signal (HIGH for read). Pins 5 through 12 comprise the parallel data bus that directly communicate with the memory.

## Important Functions ##
The EZPROM library has functions at many different layers to aid in communication of all sorts. At the most fundamental level is the single sub-bit function void pulseClock(int delay), which simply spikes and drops the clock line. This is useful when shifting data through the shift registers. There are no other instructions at the sub-bit level, and also no instructions at the bit level, since the Arduino library takes care of these.

Two byte-level instructions are included: void sendByte(byte b), and byte readByte(void), and they perform exactly as expected.

At the bytestream level, void sendByteStream(byte* m). This will take
