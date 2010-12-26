#include "IO/fread.h"

size_t fread_withState(void *in_dstBuffer, size_t in_elementSize, size_t in_count, FILE* in_file,
	void *in_pState, void (*in_pHandler)(void* in_pState, const void* in_pBuffer, size_t in_count))
{
	// TODO
	return fread(in_dstBuffer, in_elementSize, in_count, in_file);
}