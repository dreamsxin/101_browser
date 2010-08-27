#ifndef _2_4_5_h
#define _2_4_5_h

#include "MiniStdlib/cstdint.h"
#include "MiniStdlib/cstdbool.h"
#include "BigNumber/BigInteger.h"
#include "Util/Unicode.h"

#ifdef __cplusplus
extern "C" {
#endif

/* 2.4.5 Dates and times
 * Draft Standard - 4th of August 2010
 */

/*
 * For 1 <= month <= 12 this function returns the number of days in the month
 * if month is out of this interval, 0 is returned.
 */
#ifdef _WIN32
__declspec(dllexport)
#endif
uint8_t numberOfDaysInMonthOfYear(uint8_t month, UnsignedBigInteger year);

#ifdef _WIN32
__declspec(dllexport)
#endif
bool isDigit(UnicodeCodePoint in_p);

#ifdef __cplusplus
}
#endif

#endif
