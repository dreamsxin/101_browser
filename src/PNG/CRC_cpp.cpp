#include "PNG/CRC.h"
#include "Template/HigherOrderFunctions/Fold.h"

uint32_t CRC_foldl(uint32_t in_currentCRC, uint8_t *in_buffer, size_t in_bufferSize)
{
	return CRC_terminate(
		MTAx::HigherOrderFunctions::foldl<uint32_t, uint8_t>(&CRC_update, in_currentCRC, in_buffer, in_bufferSize)
		);
}
