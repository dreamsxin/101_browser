#ifndef _AbstractParser_h
#define _AbstractParser_h

#include "BasicDataStructures/Stream/BasicOpenStream.h"
#include "BasicDataStructures/Stream/BasicWriteStream.h"

class AbstractParser
{
protected:
	/** 
	 * parseToken checks whether isDone() returns true.
	 * If this is not the case this function gets called.
	 */
	virtual bool parseTokenIfNotDone(char in_token) = 0;

public:
	/**
	 * Return value: 
	 * true: token was accepted
	 * false: token was not accepted (either parsing finished or error occured)
	 */
	inline virtual bool parseToken(char in_token)
	{
		if (isDone())
			return false;
		else
			return parseTokenIfNotDone(in_token);
	}

	/**
	 * Resets the parser
	 */
	virtual void reset() = 0;

	/**
	 * Returns true if the parser won't accept any more tokens
	 */
	virtual bool isDone() const = 0;

	/**
	 * Returns true, if we could terminate parsing and get a valid result
	 */
	virtual bool isOK() const = 0;
};

#endif
