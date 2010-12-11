#ifndef _ReadResult_h
#define _ReadResult_h

typedef enum
{
	ReadResultOK,
	ReadResultPrematureEndOfStream,
	ReadResultInvalidData,
	/*
	 * ReadResultInvalidVersion is returned when it could perhaps
	 * be correct, but the claimed version of the file format is
	 * too old
	 */
	ReadResultInvalidVersion,
	ReadResultAllocationFailure,
	ReadResultNotImplemented
} ReadResult;

#endif
