#ifndef _SwfStructure_h
#define _SwfStructure_h

#include "BasicDataStructures/Stream/BasicReadStream.h"
#include <cstdlib>
#include <list>

struct RECT
{
	unsigned char NBits; // NBits UB[5]	Bits in each rect value field
	// signed int is the size required for SB[UB[5]]
#pragma pack(push, 1)
	signed int Xmin; // Xmin SB[Nbits] x minimum position for rect
	signed int Xmax; // Xmax SB[Nbits] x maximum position for rect
	signed int Ymin; // Ymin SB[Nbits] y minimum position for rect
	signed int Ymax; // Ymax SB[Nbits] y maximum position for rect
#pragma pack(pop)

	inline RECT() : Xmin(0), Xmax(0), Ymin(0), Ymax(0) { }
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
#pragma pack(push, 1)
	unsigned short FrameRate;
	unsigned short FrameCount;
#pragma pack(pop)
};

struct RECORDHEADER
{
	unsigned short TagType; // 10 Bits
	signed int Length;      // 6 bits or SI32

	RECORDHEADER() : TagType(0), Length(0) { }
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
