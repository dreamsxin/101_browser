#ifndef _ASCIIStringUnicodeIterator_h
#define _ASCIIStringUnicodeIterator_h

#include "Util/Iterator.h"
#include "Util/Unicode.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	// INV:ASCIIStringUnicodeIterator_h_13: characterPointer != NULL
	// INV:ASCIIStringUnicodeIterator_h_14: *characterPointer != 0
	char* characterPointer;
	UnicodeCodePoint codePoint;
} ASCIIStringUnicodeIteratorState;

#ifdef _WIN32
__declspec(dllexport)
#endif
SingleIterator asciiStringUnicodeIterator_create();

/*
 * PRE:ASCIIStringUnicodeIterator_h_23: string != NULL
 * PRE:ASCIIStringUnicodeIterator_h_24: *string != 0
 * 
 * POST:ASCIIStringUnicodeCyclicIterator_h_28: the result obeys INV:ASCIIStringUnicodeIterator_h_13
 * POST:ASCIIStringUnicodeCyclicIterator_h_29: the result obeys INV:ASCIIStringUnicodeIterator_h_14
 */
#ifdef _WIN32
__declspec(dllexport)
#endif
ASCIIStringUnicodeIteratorState asciiStringUnicodeIteratorState_create(char* string);

#ifdef __cplusplus
}
#endif

#endif
