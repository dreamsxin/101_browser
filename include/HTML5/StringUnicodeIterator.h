#ifndef _StringUnicodeIterator_h
#define _StringUnicodeIterator_h

#include "Util/Iterator.h"
#include "Util/Unicode.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _StringUnicodeIteratorState
{
	char* characterPointer;
	UnicodeCodePoint codePoint;
} StringUnicodeIteratorState;

SingleIterator stringUnicodeIterator_create();

StringUnicodeIteratorState stringUnicodeIteratorState_create(char* string);

#ifdef __cplusplus
}
#endif

#endif
