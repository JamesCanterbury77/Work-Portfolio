/*
    Utility functions (e.g., for bit/byte manipulations) used in Y86 simulator.
    
    Author: James Canterbury    
    version: 1.0
*/
#include <cstdlib>
#include <iostream>
#include <cassert>
#include <cstdint>
#include "Tools.h"

namespace Tools {
    
/*-----------------------------------------------------------------------------------------------
    getBits
	Extracts bits 'low' to 'high' (inclusive) from 'source' and returns the result as
	an 64-bit integer. 
	Example:
	
	getBits(12,15,0x0123456789abcdef);
	
	would return the value:
	
	0x00000000000000c;
	
	(Bits 12-15 of the 64-bit word, counting from the right correspond to hex digit 'c' in the
	source word. Note that any number of bits from 1 to 64 could be specified depending on the values
	of 'low' and 'high'.
	The value of high must be greater than or equal to low and high must be in the range 0 to 63. 
------------------------------------------------------------------------------------------------*/
uint64_t getBits(unsigned low, unsigned high, uint64_t source)
{
    assert(high < 64 && (low <= high));
    uint64_t x = -1UL;
    source = source >> low;
    uint64_t distance = (high - low) + 1;
    x = x << distance;
    if (distance != 64) {
        x = ~x;
    }
    source = source & x;
    return source;
}
/*-----------------------------------------------------------------------------------------------
    setBits
	Sets (to 1) the bits from bit numbers 'low' to 'high' (inclusive) in 
	the source word and returns the resulting word.
	
	The value of high must be greater than or equal to low and high must be in the range 0 to 63. 
------------------------------------------------------------------------------------------------*/
uint64_t setBits(unsigned low, unsigned high, uint64_t source)
{
    assert(high < 64 && (low <= high));
    uint64_t x = -1UL;
    unsigned distance = (high - low) + 1;
    x = x << distance;
    if (distance != 64) {
        x = ~x;
    }
    x = x << low;
    return source | x;
}
/*-----------------------------------------------------------------------------------------------
    clearBits
	
	Clears (to 0) the bits from bit numbers 'low' to 'high' (inclusive) in 
	the source word and returns the resulting word.
	
	The value of high must be greater than or equal to low and high must be in the range 0 to 63. 
------------------------------------------------------------------------------------------------*/
uint64_t clearBits(unsigned low, unsigned high, uint64_t source)
{
    assert(high < 64 && (low <= high));
    uint64_t x = -1UL;
    unsigned distance = (high - low) + 1;
    x = x << distance;
    if (distance != 64) {
        x = ~x;
    }
    x = x << low;
    x = ~x;   
    return source & x;
}

/*-----------------------------------------------------------------------------------------------
    assignOneBit
	Sets the bit number 'bitNum' to the binary value (0 or 1) specified by 'bitVal' in
	the source word and returns the resulting word.
	
	'bitNum' must be in the range 0 to 63 (inclusive) and 'bitVal' must be 0 or 1.
------------------------------------------------------------------------------------------------*/
uint64_t assignOneBit(unsigned bitNum, unsigned bitVal, uint64_t source)
{
    assert(bitNum < 64 && ((bitVal == 0) | (bitVal == 1)));
    uint64_t x = 0;
    if (bitVal == 0) {
        x = x >> bitNum;
        x = x + 1;
        x = x << bitNum;
        x = ~x;
        source = source & x;
    } else {
        x = 0;
        x = x + 1;
        x = x << bitNum;
        source = source | x;
    }
    return source;
}

/*-----------------------------------------------------------------------------------------------
    getByteNumber
	Returns the specified byte number from the source word.
	
	Example:
	getByteNumber(3,0x0011223344556677);
	
	will return the single byte value:
	
	0x44
	
	(Note that the '3' refers to byte 3 of a 8-byte unsigned integer (bytes are numbered from
	the right starting at 0 when looking at a word value in hex). 
------------------------------------------------------------------------------------------------*/
uint8_t getByteNumber(unsigned byteNum, uint64_t source)
{ 
    assert(byteNum < (64/8));
    uint64_t x = -1UL;
    unsigned bits = byteNum * 8;
    source = source >> bits;
    x = x << 8;
    x = ~x; 
    return source & x;
}
/*-----------------------------------------------------------------------------------------------
    putByteNumber
	Returns the source word after replacing the specified byte (byteNum) with a given 
	byte value (byteVal).
	
	Example:
	putByteNumber(3,0x00,0x0011223344556677);
	
	will return the value:
	
	0x0011223300556677
	
	(Note that the '3' refers to byte 3 of a 8-byte unsigned integer (bytes are numbered from
	the right starting at 0 when looking at a word value in hex). 
------------------------------------------------------------------------------------------------*/
uint64_t putByteNumber(unsigned byteNum, uint8_t byteVal, uint64_t source)
{
    assert(byteNum < (64/8));
    //removes byte from source
    unsigned bits = byteNum * 8;
    uint64_t x = -1UL;
    x = x << 8;
    x = ~x;
    x = x << bits;
    x = ~x;
    source = source & x;
    //adds byteVal to source
    uint64_t y = (uint64_t) byteVal;
    y = y << bits;
    return source | y;                  
}
/*-----------------------------------------------------------------------------------------------
    buildWord
	
	Returns a 64 bit word consisting of the 8 parameter bytes, where b0 is the least significant
	and b7 is the most significant byte.
------------------------------------------------------------------------------------------------*/
uint64_t buildWord(unsigned char b0, unsigned char b1,unsigned char b2, unsigned char b3,
                          unsigned char b4, unsigned char b5,unsigned char b6, unsigned char b7)
{
    uint64_t x = 0;
    x = x | b7;
    x = x << 8;
    x = x | b6;
    x = x << 8;
    x = x | b5;
    x = x << 8;                     
    x = x | b4;
    x = x << 8;
    x = x | b3;
    x = x << 8;
    x = x | b2;
    x = x << 8;
    x = x | b1;
    x = x << 8;
    x = x | b0;
    return x;
}
/*-----------------------------------------------------------------------------------------------
    isNegative

	Returns true if the input parameter is negative when interpreted as a signed value.
------------------------------------------------------------------------------------------------*/
bool isNegative(uint64_t source)
{
    
    source = source >> 63;
    if (source == 1) {
        return true;
    }
    return false;
}
/*-----------------------------------------------------------------------------------------------
    expandBits
	
	Reads the source and converts it to a character string consisting of 72 bytes--71 characters
	followed by a '\0' character (string terminator).
	Example usage: 
		char buf[72];
		expandBits(0xAABBCCDDEE001122UL,buf);

		Will cause the following string to be written to the buf[] array (not including quotes):
	
	"10101010 10111011 11001100 11011101 11101110 00000000 00010001 00100010"
	
	Note the space after every group of 8 digits except the last. The last byte of
	the string is the '\0' byte, that is not printable but used to indicate the end
	of a C string.
------------------------------------------------------------------------------------------------*/
void expandBits(uint64_t source, char *bits)
{
    /*
    uint64_t x = -1UL;
    x = x << 1;
    x = ~x;
    x = x << 63;
    uint64_t temp = source;
    int j = 8;
    for (int i = 0; i < 70; i++) {
        if (i == j) {
            bits[i] = ' ';
            j += 9;
        } else {
            temp = temp & x;
            if (i < 63) {
                temp = temp >> (63 - i);
            } else {
                i = 63 - 
            }
            if (temp == 0) {
                bits[i] = '0';
            } else {
                bits[i] = '1';
            }
            x = x >> 1;
            temp = source;
        }
    }
    bits[72] = '\0';
    return;
    */
    assert(bits != NULL);
    uint64_t x = -1UL;
    x = x << 1;
    x = ~x;
    x = x << 63;
    uint64_t temp = source;
    int j = 8;
    int location = 0;
    for (int i = 0; i < 63; i++) {
        if (i == j) {
            bits[location] = ' ';
            location++;
            j += 8;
            i--;
        } else {
            temp = temp & x;
            temp = temp >> (63 - i);
            if (temp == 0) {
                bits[location] = '0';
            } else {
                bits[location] = '1';
            }
            x = x >> 1;
            temp = source;
            location++;
        }
    }
    uint64_t a = 1;
    source = source & a;
    if (source == 0) {
        bits[70] = '0';
    } else {
        bits[70] = '1';
    }
    bits[71] = '\0';
    return;

}
/*-----------------------------------------------------------------------------------------------
    clearBuffer
	Treats pbuf as a pointer to an array of 'size' bytes and clears them all to zero.
------------------------------------------------------------------------------------------------*/
void clearBuffer(char * pbuf, int size)
{
    assert(pbuf != NULL && size >= 0 && size <= (2147483647));
    for (int i = 0; i < size; i++) {
        pbuf[i] = 0;       
    }
    return;
}

} // end namespace Tools
