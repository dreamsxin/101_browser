#include "Unicode/PropList.h"
#include "Util/Interval.hpp"
#include "Util/Unicode.h"
#include <cstdlib> // for malloc
#include <list>
#include <stdexcept>
using namespace std;


enum PropListState
{
	// Initial state
	BeginOfLine,

	// Terminal states
	ProperlyTerminated,
	Error,

	// Other States
	FirstNumber,
	Separator,
	BeforeSecondNumber,
	SecondNumber,
	SpacesAfterNumber,
	SpaceBeforeProperty,
	Property,
	SpaceAfterProperty,
	Comment
};

bool readPropList(FILE* in_file, char* in_property, void* out_ppIntervals, size_t* out_intervalsCount)
{
	fpos_t position;
	if (fgetpos(in_file, &position) != 0)
		return false;

	fseek(in_file, 0, SEEK_SET);

	Interval<UnicodeCodePoint>* pIntervals = NULL;
	std::list<Interval<UnicodeCodePoint> > listIntervals;

	unsigned char token;
	PropListState currentState = BeginOfLine;
	UnicodeCodePoint firstNumber;
	UnicodeCodePoint secondNumber;
	char* propertyPos;
	bool propertyEquals;

	while (currentState != ProperlyTerminated && currentState != Error)
	{
		if (fread(&token, 1, 1, in_file) != 1)
		{
			if (currentState == BeginOfLine)
				currentState = ProperlyTerminated;
			else
				currentState = Error;

			continue;
		}

		switch (currentState)
		{
		case BeginOfLine:
			if ((token >= '0' && token <= '9') || (token >= 'A' && token <= 'F'))
			{
				firstNumber = readHexDigit(token);
				currentState = FirstNumber;
			}
			else if (token == '#')
			{
				currentState = Comment;
			}
			else if (token == '\n')
			{
				// nothing
			}
			else
			{
				currentState = Error;
			}
			break;
		case FirstNumber:
			if ((token >= '0' && token <= '9') || (token >= 'A' && token <= 'F'))
			{
				firstNumber = 16 * firstNumber + readHexDigit(token);
			}
			else if (token == '.')
			{
				currentState = Separator;
			}
			else if (token == ' ')
			{
				secondNumber = firstNumber;
				currentState = SpacesAfterNumber;
			}
			else
			{
				currentState = Error;
			}
			break;
		case Separator:
			if (token == '.')
			{
				currentState = BeforeSecondNumber;
			}
			else
			{
				currentState = Error;
			}
			break;
		case BeforeSecondNumber:
			if ((token >= '0' && token <= '9') || (token >= 'A' && token <= 'F'))
			{
				secondNumber = readHexDigit(token);
				currentState = SecondNumber;
			}
			else
			{
				currentState = Error;
			}
			break;
		case SecondNumber:
			if ((token >= '0' && token <= '9') || (token >= 'A' && token <= 'F'))
			{
				secondNumber = 16*secondNumber + readHexDigit(token);
				currentState = SecondNumber;
			}
			else if (token == ' ')
			{
				currentState = SpacesAfterNumber;
			}
			else if (token == ';')
			{
				currentState = SpaceBeforeProperty;
			}
			else
			{
				currentState = Error;
			}
			break;
		case SpacesAfterNumber:
			if (token == ' ')
			{
				// nothing
			}
			else if (token == ';')
			{
				currentState = SpaceBeforeProperty;
			}
			else
			{
				currentState = Error;
			}
			break;
		case SpaceBeforeProperty:
			if (token == ' ')
			{
				currentState = Property;
				propertyPos = in_property;
				propertyEquals = true;
			}
			else
			{
				currentState = Error;
			}
			break;
		case Property:
			if ((token >= 'A' && token <= 'Z') || (token >= 'a' && token <= 'z') || (token == '_'))
			{
				if (*propertyPos != 0 && propertyEquals && token == *propertyPos)
					propertyPos++;
				else
					propertyEquals = false;
			}
			else if (token == ' ')
			{
				if (*propertyPos)
					propertyEquals = false;

				if (propertyEquals)
				{
					try
					{
						listIntervals.push_back(createInterval<UnicodeCodePoint>(firstNumber, secondNumber));
					}
					catch (bad_alloc&)
					{
						fsetpos(in_file, &position);
						return 0;
					}
				}

				currentState = SpaceAfterProperty;
			}
			else
			{
				currentState = Error;
			}
			break;
		case SpaceAfterProperty:
			if (token == '#')
			{
				currentState = Comment;
			}
			else
			{
				currentState = Error;
			}
			break;
		case Comment:
			if (token == '\n')
			{
				currentState = BeginOfLine;
			}
			// no else here
		}
	}

	if (currentState == ProperlyTerminated)
	{
		pIntervals = (Interval<UnicodeCodePoint>*) malloc(sizeof(Interval<UnicodeCodePoint>) * listIntervals.size());

		if (pIntervals == NULL)
		{
			fsetpos(in_file, &position);
			return false;
		}

		size_t index = 0;

		for (list<Interval<UnicodeCodePoint> >::iterator i = listIntervals.begin(); i != listIntervals.end(); ++i)
		{
			pIntervals[index] = *i;
			index++;
		}

		*((Interval<UnicodeCodePoint>**) out_ppIntervals) = pIntervals;
		*out_intervalsCount = index;

		fsetpos(in_file, &position);
		return true;
	}
	else
	{
		fsetpos(in_file, &position);
		return false;
	}
}
