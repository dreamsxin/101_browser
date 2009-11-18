#ifndef _SwfStructure_h
#define _SwfStructure_h

#include "BasicDataStructures/Stream/BasicReadStream.h"
#include <cstdlib>
#include <list>

struct RECT
{
	unsigned char NBits; // UB[5]	Bits in each rect value field
	// Xmin SB[Nbits] x minimum position for rect
	// Xmax SB[Nbits] x maximum position for rect
	// Ymin SB[Nbits] y minimum position for rect
	// Ymax SB[Nbits] y maximum position for rect
	unsigned char* XminXmaxYminYmax;

	inline RECT() : XminXmaxYminYmax(NULL) { }
	inline ~RECT()
	{
		delete[] XminXmaxYminYmax;
	}
};

/*!
 * Reads a RECT.
 *
 * Return value: 
 * true on success
 * false on failure
 */
bool readRECT(BasicReadStream* in_pStream, RECT* in_pRect);

struct SwfHeader1
{
	char Signature[3];
	unsigned char Version;
	unsigned int FileLength;
};

struct SwfHeader2
{
	RECT FrameSize;
	unsigned short FrameRate;
	unsigned short FrameCount;
};

struct RECORDHEADER
{
	unsigned char TagCode;
	signed int Length;
};

struct Tag
{
	RECORDHEADER recordHeader;
	unsigned char* tagContent;

	inline Tag() : tagContent(NULL) { }
};

struct SwfFile
{
	SwfHeader1 swfHeader1;
	SwfHeader2 swfHeader2;
	std::list<Tag> tags;
};

#endif
