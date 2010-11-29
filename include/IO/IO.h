#ifndef _IO_h
#define _IO_h

typedef struct
{
	bool (*pGetPos)(const void* in_pIOState, void* in_pPosState);
	bool (*pSetPos)(const void* in_pIOState, const void* in_pPosState);

	size_t (*pRead)(const void* in_pIOState, size_t in_size, void* in_pBuffer);

	void (*pClose)(void* in_pIOState);

} IO_Interface;

#endif
