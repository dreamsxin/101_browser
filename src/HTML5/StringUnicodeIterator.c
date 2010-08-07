#include "HTML5/StringUnicodeIterator.h"

/*//void* stringUnicodeIterator_get

SingleIterator stringUnicodeIterator_create()
{

}*/

StringUnicodeIteratorState stringUnicodeIteratorState_create(char* string)
{
	StringUnicodeIteratorState out_state = {string, 0};
	return out_state;
}
