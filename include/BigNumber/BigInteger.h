/*
 * Copyright 2008-2011 Wolfgang Keller
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _BigInteger_h
#define _BigInteger_h

#include "MiniStdlib/cstdint.h"
#include "MiniStdlib/cstdbool.h"
#include "MiniStdlib/declspec.h"

// for size_t
#ifdef __cplusplus
#include <cstdlib>
#else
#include <stdlib.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _UnsignedBigInteger
{
	size_t numberSize;
	size_t allocedLimbsCount;
	uint32_t* limbs;
} UnsignedBigInteger;


DLLEXPORT bool initUnsignedBigIntegerUC(UnsignedBigInteger* in_pNumber, uint32_t in_value);

/*
 * Creates a number using the passed limbs. Note that we will allocate 
 * limbsCount many uint32_t although the number could be shorter (last passed
 * limbs zero). This can (amd should) be used to tell the implementation
 * when to allocate more memory than necessary.
 * 
 * Return value: 1 on success
 *               0 on failure
 */
DLLEXPORT bool initUnsignedBigIntegerUCA(UnsignedBigInteger* in_pNumber, 
							   uint32_t* limbs, size_t limbsCount);

/*
 * Deallocates the used memory and sets the number to zero
 */
DLLEXPORT void freeUnsignedBigInteger(UnsignedBigInteger* in_pNumber);

struct SignedBigInteger
{
	UnsignedBigInteger absoluteValue;
	bool isNegative;
};

DLLEXPORT bool isZero(UnsignedBigInteger const * const in_pcInt);

DLLEXPORT uint32_t incUB(UnsignedBigInteger* in_pInt);

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
DLLEXPORT uint32_t addUBUC(UnsignedBigInteger* in_pInt, uint32_t in_c);

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
DLLEXPORT uint32_t mulUBUC(UnsignedBigInteger* in_pInt, uint32_t in_c);

DLLEXPORT uint32_t modUBUC(UnsignedBigInteger const * const  in_pInt, uint32_t in_c);

/* Internal helper functions */
uint32_t applyCarry(UnsignedBigInteger* in_pInt, uint32_t in_c);
bool addUBUCKernel(uint32_t* in_pCarry, uint32_t* in_pCurrentDigit);
uint32_t mulUBUCKernel(uint32_t in_carry, uint32_t* in_pCurrentDigit, uint32_t in_n);
uint32_t modUBUCKernel(uint32_t in_carry, uint32_t* in_pCurrentDigit, uint32_t in_n);

#ifdef __cplusplus
}
#endif

#endif
