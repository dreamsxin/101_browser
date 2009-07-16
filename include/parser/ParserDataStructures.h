#ifndef _ParserDataStructures_h
#define _ParserDataStructures_h

struct ReadByte
{
	bool endOfStream;    // if true, the stream has finished and the value of token is
	                     // undefined
	// here some padding bytes might occur
	unsigned char token; // if endOfStream==false the value of the token is stored here
	// here some padding bytes might occur
};

const unsigned char cStatusOK = 0;
const unsigned char cStatusErrorInvalidData = 1; // we got invalid data - so we could not handle it

template<typename T> struct Result
{
	unsigned char status;
	T result;             // this is the result; if status!=0 its value is not defined
};

#endif
