#ifndef _ASCIIStringUnicodeIterator_h
#define _ASCIIStringUnicodeIterator_h

#include "Util/Iterator.h"
#include "Util/Unicode.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _ASCIIStringUnicodeIteratorState
{
	char* characterPointer;
	UnicodeCodePoint codePoint;
} ASCIIStringUnicodeIteratorState;

#ifdef _WIN32
__declspec(dllexport)
#endif
SingleIterator asciiStringUnicodeIterator_create();

#ifdef _WIN32
__declspec(dllexport)
#endif
ASCIIStringUnicodeIteratorState asciiStringUnicodeIteratorState_create(char* string);

#ifdef __cplusplus
}
#endif

#endif
