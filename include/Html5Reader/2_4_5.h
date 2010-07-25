#ifndef _2_4_5_h
#define _2_4_5_h

#include "MiniStdlib/cstdint.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * TODO: Replace year by value of unlimited precision
 *
 * Precondition: 1 <= month <= 12
 */
uint8_t numberOfDaysInMonthOfYear(uint8_t month, uint32_t year);

#ifdef __cplusplus
}
#endif

#endif
