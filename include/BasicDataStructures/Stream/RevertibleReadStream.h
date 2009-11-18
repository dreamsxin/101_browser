#ifndef _RevertibleReadStream_h
#define _RevertibleReadStream_h

#include "BasicDataStructures/Stream/BasicReadStream.h"

/*!
 * A stream which can also run "backward"
 */
class RevertibleReadStream : public BasicReadStream
{
public:
	virtual void revert(long in_offset) = 0;
};


#endif
