#ifndef _MTAx_Util_ConvertResult_h
#define _MTAx_Util_ConvertResult_h

typedef enum
{
	/*!
	 * All data has been read and converting has terminated succesfully.
	 * If there were additional data it would surely lead to a failure, 
	 * so we force termination.
	 */
	ConvertResultTerminateOnly,
	/*!
	 * All data has been read and converting has terminated succesfully.
	 * We won't exclude that additional data would also lead to a correct
	 * (normally longer) converted stream, so we may continue, if we want to.
	 */
	ConvertResultTerminationPossible,
	/*!
	 * The cúrrent state does not allow termination, but we can't exclude that
	 * delivering additional data could lead to a valid conversation result.
	 * So please deliver some
	 */
	ConvertResultAdditionalDataNeeded,

	ConvertResultAllocationFailure
} ConvertResult;

#endif
