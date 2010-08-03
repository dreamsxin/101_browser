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
} UnsignedBigInteger;

/*
 * Return value: 1 on success
 *               0 on failure
 */
#ifdef _WIN32
__declspec(dllexport)
#endif
uint8_t initUnsignedBigInteger(UnsignedBigInteger* in_pNumber, 
							   uint32_t* limbs, size_t limbsCount);

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
