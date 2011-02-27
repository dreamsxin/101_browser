#ifndef _MTAx_Util_ReadResult_h
#define _MTAx_Util_ReadResult_h

typedef enum
{
	ReadResultOK,
	ReadResultPrematureEndOfStream,
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
	* When we don't know what kind of error it is we use this. Should
	* be avoided if possible.
	*/
	ReadResultSomeError
} ReadResult;

#endif
