#ifndef _BitRead_h
#define _BitRead_h

#ifdef __cplusplus
#include <cstdio>
#else
#include <stdio.h>
#endif

#include "MiniStdlib/cstdint.h"
#include "MiniStdlib/cstdbool.h"
 // for size_t
#ifdef __cplusplus
#include <cstddef>
#else
#include <stddef.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	/*
	 * This field contains a value from 0 to 7 and tells how many
	 * bits are stored in the buffer. Temporarily it can also take
	 * the value 8.
	 */
	uint8_t bitCountInBuffer;
	/*
	 * Up to 7 bits (temporarily 8) are buffered here. This variable
	 * acts as a kind of binary shift register.
	 */
	uint8_t buffer;
} BitReadState;

void initBitReadState(BitReadState *in_pBitReadState);
/*!
 * Reads in_bitsCount bits from the file.
 * 
 * Return value:
 * true if successful
 * false if not
 */
bool readBits(BitReadState *in_pBitReadState, void* in_pBuffer, size_t in_bitsCount, FILE* in_file);

#ifdef __cplusplus
}
#endif

#endif
