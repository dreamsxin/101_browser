#ifndef _AbstractParser_h
#define _AbstractParser_h

#include "BasicDataStructures/Stream/BasicOpenStream.h"
#include "BasicDataStructures/Stream/BasicWriteStream.h"

template <typename OpenType> class AbstractParser : 
public BasicOpenStream<OpenType>
{
public:
	/**
	 * Return value: 
	 * true: token was accepted
	 * false: token was not accepted (either parsing finished or error occured)
	 */
	virtual bool parseToken(char in_token) = 0;
};

#endif
