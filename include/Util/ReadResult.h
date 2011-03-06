#ifndef _MTAx_Util_ReadResult_h
#define _MTAx_Util_ReadResult_h

typedef enum
{
	ReadResultOK,

	ReadResultPrematureEndOfStream,
	/*
	* Exactly the opposite of ReadResultPrematureEndOfStream:
	* an end was expected (and is normally necessary to be checked!),
	* but did not occur.
	*/
	ReadResultOverdueEndOfStream,

	/*
	* ReadResultInvalidVersion is returned when the read data could 
	* be correct, but the claimed version of the file format does not
	* fit.
	*/
	ReadResultInvalidVersion,
	ReadResultInvalidData,
	ReadResultAllocationFailure,
	ReadResultNotImplemented,

	/*
	* This normally means that a passed parameter does not
	* comply to the postulated invarint. So it is a sign
	* of a serious bug.
	*/
	ReadResultInternalInconsistency,

	/*
	* When we don't know what kind of error it is we use this. Should
	* be avoided if possible.
	*/
	ReadResultSomeError
} ReadResult;

#endif
