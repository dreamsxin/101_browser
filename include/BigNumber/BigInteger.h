#ifndef _BigInteger_h
#define _BigInteger_h

#include "MiniStdlib/cstdint.h"
#include "MiniStdlib/cstdbool.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _UnsignedBigInteger
{
	size_t numberSize;
	size_t allocedLimbsCount;
	uint32_t* limbs;
} UnsignedBigInteger;


#ifdef _WIN32
__declspec(dllexport)
#endif
bool initUnsignedBigIntegerUC(UnsignedBigInteger* in_pNumber, uint32_t in_value);

/*
 * Creates a number using the passed limbs. Note that we will allocate 
 * limbsCount many uint32_t although the number could be shorter (last passed
 * limbs zero). This can (amd should) be used to tell the implementation
 * when to allocate more memory than necessary.
 * 
 * Return value: 1 on success
 *               0 on failure
 */
#ifdef _WIN32
__declspec(dllexport)
#endif
bool initUnsignedBigIntegerUCA(UnsignedBigInteger* in_pNumber, 
							   uint32_t* limbs, size_t limbsCount);

/*
 * Deallocates the used memory and sets the number to zero
 */
#ifdef _WIN32
__declspec(dllexport)
#endif
void freeUnsignedBigInteger(UnsignedBigInteger* in_pNumber);

struct SignedBigInteger
{
	UnsignedBigInteger absoluteValue;
	bool isNegative;
};

#ifdef _WIN32
__declspec(dllexport)
#endif
bool isZero(UnsignedBigInteger const * const in_pcInt);

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

/* An internal helper function */
uint32_t applyCarry(UnsignedBigInteger* in_pInt, uint32_t in_c);

#ifdef __cplusplus
}
#endif

#endif