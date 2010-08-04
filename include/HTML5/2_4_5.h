#ifndef _2_4_5_h
#define _2_4_5_h

#include "MiniStdlib/cstdint.h"
#include "BigNumber/BigInteger.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * TODO: Replace year by value of unlimited precision
 *
 * Precondition: 1 <= month <= 12
 */
#ifdef _WIN32
__declspec(dllexport)
#endif
uint8_t numberOfDaysInMonthOfYear(uint8_t month, UnsignedBigInteger year);

#ifdef __cplusplus
}
#endif

#endif
