#ifndef _BigInteger_h
#define _BigInteger_h

#include <cstdlib>

struct UnsignedBigInteger
{
	size_t numberSize;
	size_t allocedLimbsCount;
	unsigned long* limbs;

	inline UnsignedBigInteger() : 
	numberSize(0), allocedLimbsCount(0), limbs(NULL) { }
};

struct SignedBigInteger
{
	UnsignedBigInteger absoluteValue;
	bool isNegative;
};

bool isZero(UnsignedBigInteger const * const in_pcInt);

/*!
 * Tries to increment the value pointed by in_pInt
 * 
 * Return value: true if succeeded
 *               false otherwise
 *
 * The reasons why it may returns false are
 * 1. a memory allocation failed
 * 2. in_pInt->numberSize >= SIZE_MAX-1
 */
bool inc(UnsignedBigInteger* in_pInt);

/*!
 * Tries to decrement the value pointed by in_pInt
 *
 * Return value: true if succeeded
 *               false otherwise
 *
 * The reasons why it may returns false are
 * 1. the value pointed by in_pInt is already zero
 *    (this can be detected by isZero)
 * 2. a realloc to a smaller block size fails (this should
 *    never happen, but how sure can you be :-( )
 */
bool dec(UnsignedBigInteger* in_pInt);

#endif
