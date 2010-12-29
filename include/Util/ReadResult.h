#ifndef _MTAx_Util_ReadResult_h
#define _MTAx_Util_ReadResult_h

typedef enum
{
	ReadResultOK,
	ReadResultPrematureEndOfStream,
	/*
	 * ReadResultInvalidVersion is returned when the read data could 
	 * be correct, but the claimed version of the file format does not
	 * fit
	 */
	ReadResultInvalidVersion,
	ReadResultInvalidData,
	ReadResultAllocationFailure,
	ReadResultNotImplemented
} ReadResult;

#endif
