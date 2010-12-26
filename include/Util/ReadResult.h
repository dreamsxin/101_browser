#ifndef _ReadResult_h
#define _ReadResult_h

typedef enum
{
	ReadResultOK,
	ReadResultPrematureEndOfStream,
	/*
	 * This is returned if the data is invalid, but we can continue
	 * with probably no problems (for example: wrong checksum)
	 */
	ReadResultInvalidDataButRecoverable,
	/*
	 * ReadResultInvalidVersion is returned when it could perhaps
	 * be correct, but the claimed version of the file format is
	 * too old
	 */
	ReadResultInvalidVersion,
	ReadResultInvalidData,
	ReadResultAllocationFailure,
	ReadResultNotImplemented
} ReadResult;

#endif
