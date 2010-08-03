#ifndef _BigInteger_h
#define _BigInteger_h

#include "MiniStdlib/cstdint.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _UnsignedBigInteger
{
	size_t numberSize;
	size_t allocedLimbsCount;
	uint32_t* limbs;

	/*inline UnsignedBigInteger() : 
	numberSize(0), allocedLimbsCount(0), limbs(NULL) { }*/
} UnsignedBigInteger;

struct SignedBigInteger
{
	UnsignedBigInteger absoluteValue;
	uint8_t isNegative;
};

#ifdef _WIN32
__declspec(dllexport)
#endif
uint8_t isZero(UnsignedBigInteger const * const in_pcInt);

#ifdef _WIN32
__declspec(dllexport)
#endif
uint32_t incUB(UnsignedBigInteger* in_pInt);

/*!
 * Adds in_c to in_pInt.
 * 
 * Return value:
 * 0 if sucessfull
 * other value: either the size of the number is too large or
 *              memory could not be allocated. In this case we
 *              return the value that would go into the highest
 *              digit
 */
#ifdef _WIN32
__declspec(dllexport)
#endif
uint32_t addUBUC(UnsignedBigInteger* in_pInt, uint32_t in_c);

/*!
 * Multiplies in_pInt with in_c.
 * 
 * Return value:
 * 0 if sucessfull
 * other value: either the size of the number is too large or
 *              memory could not be allocated. In this case we
 *              return the value that would go into the highest
 *              digit
 */
#ifdef _WIN32
__declspec(dllexport)
#endif
uint32_t mulUBUC(UnsignedBigInteger* in_pInt, uint32_t in_c);

#ifdef _WIN32
__declspec(dllexport)
#endif
uint32_t modUBUC(UnsignedBigInteger const * const  in_pInt, uint32_t in_c);

// An internal helper function
uint32_t applyCarry(UnsignedBigInteger* in_pInt, uint32_t in_c);

#ifdef __cplusplus
}
#endif

#endif
