#ifndef _AbstractParser_h
#define _AbstractParser_h

#include "BasicDataStructures/Stream/BasicOpenStream.h"
#include "BasicDataStructures/Stream/BasicWriteStream.h"

template <typename OpenType> class AbstractParser : 
public BasicOpenStream<OpenType>, public BasicWriteStream
{
public:

};

#endif
