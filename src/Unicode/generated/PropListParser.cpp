#include "Unicode/PropList.h"
#include "Util/Interval.hpp"
#include "Util/Unicode.h"
#include <cstdlib> // for malloc
#include <list>
#include <stdexcept>
using namespace std;

enum PropListParserState
{
	PropListParserStateFirstNumber,
	PropListParserStateBeginOfLine,
	PropListParserStateProperlyTerminated,
	PropListParserStateInSeparator,
	PropListParserStateComment,
	PropListParserStateError,
	PropListParserStateSpacesAfterNumber,
	PropListParserStateBeforeSecondNumber,
	PropListParserStateSecondNumber,
	PropListParserStateBeforeSpaceBeforeProperty,
	PropListParserStateSpaceAfterProperty,
	PropListParserStateProperty,
	PropListParserStateTerminate
};

bool readPropList(FILE* in_file, char* in_property, void* out_ppIntervals, size_t* out_intervalsCount)
{
	// Declarations of variables
	PropListParserState lCurrentState;
	fpos_t lPosition;
	bool lContinueLoop = true;
	bool lSuccess;

	// Initialization
	if (fgetpos(in_file, &lPosition) != 0)
		return false;

	fseek(in_file, 0, SEEK_SET);

	// Transition code
	Interval<UnicodeCodePoint>* pIntervals = NULL;
	std::list<Interval<UnicodeCodePoint> > listIntervals;
	UnicodeCodePoint firstNumber;
	UnicodeCodePoint secondNumber;
	char* propertyPos;
	bool propertyEquals;

	// Initialize initial state
	lCurrentState = PropListParserStateBeginOfLine;

	while (lContinueLoop)
	{
		// Declarations of variables
		unsigned char lToken;
		bool lEndOfStream = false;

		// Parser code
		if (fread(&lToken, 1, 1, in_file) != 1)
			lEndOfStream = true;

		switch (lCurrentState)
		{
			case PropListParserStateFirstNumber:
				if (lEndOfStream)
				{
					lCurrentState = PropListParserStateError;
					break;
				}
				if (lToken == '.')
				{
					lCurrentState = PropListParserStateInSeparator;
				}
				else if (lToken == ' ')
				{
					secondNumber = firstNumber;
					lCurrentState = PropListParserStateSpacesAfterNumber;
				}
				else if ((lToken >= '0' && lToken <= '9') || (lToken >= 'A' && lToken <= 'F'))
				{
					firstNumber = 16*firstNumber + readHexDigit(lToken);
					// internal transition - no state change
				}
				else if (lToken == ';')
				{
					lCurrentState = PropListParserStateBeforeSpaceBeforeProperty;
				}
				else
				{
					lCurrentState = PropListParserStateError;
				}
				break;
			case PropListParserStateBeginOfLine:
				if (lEndOfStream)
				{
					lCurrentState = PropListParserStateProperlyTerminated;
					break;
				}
				if ((lToken >= '0' && lToken <= '9') || (lToken >= 'A' && lToken <= 'F'))
				{
					firstNumber = readHexDigit(lToken);
					lCurrentState = PropListParserStateFirstNumber;
				}
				else if (lToken == '#')
				{
					lCurrentState = PropListParserStateComment;
				}
				else if (lToken == '\n')
				{
					// internal transition - no state change
				}
				else
				{
					lCurrentState = PropListParserStateError;
				}
				break;
			case PropListParserStateProperlyTerminated:
				pIntervals = (Interval<UnicodeCodePoint>*) malloc(sizeof(Interval<UnicodeCodePoint>) * listIntervals.size());
				
				if (pIntervals == NULL)
				{
					lSuccess = false;
				}
				else
				{
					size_t index = 0;
				
					for (list<Interval<UnicodeCodePoint> >::iterator i = listIntervals.begin(); i != listIntervals.end(); ++i)
					{
						pIntervals[index] = *i;
						index++;
					}
				
					*((Interval<UnicodeCodePoint>**) out_ppIntervals) = pIntervals;
					*out_intervalsCount = index;
				
					lSuccess = true;
				}
				lCurrentState = PropListParserStateTerminate;
				break;
			case PropListParserStateInSeparator:
				if (lEndOfStream)
				{
					lCurrentState = PropListParserStateError;
					break;
				}
				if (lToken == '.')
				{
					lCurrentState = PropListParserStateBeforeSecondNumber;
				}
				else
				{
					lCurrentState = PropListParserStateError;
				}
				break;
			case PropListParserStateComment:
				if (lEndOfStream)
				{
					lCurrentState = PropListParserStateError;
					break;
				}
				if (lToken == '\n')
				{
					lCurrentState = PropListParserStateBeginOfLine;
				}
				break;
			case PropListParserStateError:
				lSuccess = false;
				lCurrentState = PropListParserStateTerminate;
				break;
			case PropListParserStateSpacesAfterNumber:
				if (lEndOfStream)
				{
					lCurrentState = PropListParserStateError;
					break;
				}
				if (lToken == ';')
				{
					lCurrentState = PropListParserStateBeforeSpaceBeforeProperty;
				}
				else if (lToken == ' ')
				{
					// internal transition - no state change
				}
				else
				{
					lCurrentState = PropListParserStateError;
				}
				break;
			case PropListParserStateBeforeSecondNumber:
				if (lEndOfStream)
				{
					lCurrentState = PropListParserStateError;
					break;
				}
				if ((lToken >= '0' && lToken <= '9') || (lToken >= 'A' && lToken <= 'F'))
				{
					secondNumber = readHexDigit(lToken);
					lCurrentState = PropListParserStateSecondNumber;
				}
				else
				{
					lCurrentState = PropListParserStateError;
				}
				break;
			case PropListParserStateSecondNumber:
				if (lEndOfStream)
				{
					lCurrentState = PropListParserStateError;
					break;
				}
				if ((lToken >= '0' && lToken <= '9') || (lToken >= 'A' && lToken <= 'F'))
				{
					secondNumber = 16*secondNumber + readHexDigit(lToken);
					// internal transition - no state change
				}
				else if (lToken == ' ')
				{
					lCurrentState = PropListParserStateSpacesAfterNumber;
				}
				else if (lToken == ';')
				{
					lCurrentState = PropListParserStateBeforeSpaceBeforeProperty;
				}
				else
				{
					lCurrentState = PropListParserStateError;
				}
				break;
			case PropListParserStateBeforeSpaceBeforeProperty:
				if (lEndOfStream)
				{
					lCurrentState = PropListParserStateError;
					break;
				}
				if (lToken == ' ')
				{
					propertyEquals = true;
					propertyPos = in_property;
					lCurrentState = PropListParserStateProperty;
				}
				else
				{
					lCurrentState = PropListParserStateError;
				}
				break;
			case PropListParserStateSpaceAfterProperty:
				if (lEndOfStream)
				{
					lCurrentState = PropListParserStateError;
					break;
				}
				if (lToken == '#')
				{
					lCurrentState = PropListParserStateComment;
				}
				else
				{
					lCurrentState = PropListParserStateError;
				}
				break;
			case PropListParserStateProperty:
				if (lEndOfStream)
				{
					lCurrentState = PropListParserStateError;
					break;
				}
				if ((lToken >= 'A' && lToken <= 'Z') || (lToken >= 'a' && lToken <= 'z') || lToken == '_')
				{
					if (*propertyPos != 0 && propertyEquals && lToken == *propertyPos)
						propertyPos++;
					else
						propertyEquals = false;
					// internal transition - no state change
				}
				else if (lToken == ' ')
				{
					if (*propertyPos != 0)
						propertyEquals = false;
					
					if (propertyEquals)
					{
						try
						{
							listIntervals.push_back(createInterval<UnicodeCodePoint>(firstNumber, secondNumber));
						}
						catch (bad_alloc&)
						{
							lContinueLoop = false;
							lSuccess = false;
						}
					}
					lCurrentState = PropListParserStateSpaceAfterProperty;
				}
				else
				{
					lCurrentState = PropListParserStateError;
				}
				break;
			case PropListParserStateTerminate:
				lContinueLoop = false;
				break;
		}
	}

	// Reset position in the file
	fsetpos(in_file, &lPosition);

	return lSuccess;
}
