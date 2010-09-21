#ifndef _2_4_4_1_h
#define _2_4_4_1_h

#include "BigNumber/BigInteger.h"
#include "Util/Iterator.h"

#ifdef __cplusplus
extern "C" {
#endif

enum Result
{
	ResultOK,
	ResultError,
	ResultAllocationFailure
};

#ifdef _WIN32
__declspec(dllexport)
#endif
/*!
 * Return value: ResultOK on success (in this case in_pUnsignedInteger will
 *               contain the parsed number)
 *               ResultError or ResultAllocationFailure on failure
 */
enum Result parseNonNegativeInteger(SingleIterator in_iterator, void* in_iteratorState, 
							   UnsignedBigInteger* in_pUnsignedInteger);

#ifdef __cplusplus
}
#endif

#endif
