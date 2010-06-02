#ifndef _FontServer_h
#define _FontServer_h

#include "BasicDataStructures/Types.h"
#include "BasicDataStructures/Memory/ArrayBlock.h"

// Pack structures to 1-byte boundaries
#pragma pack(push, 1)

#define CHAR4_TO_UINT_LIL_ENDIAN(a, b, c, d) (((unsigned char) a)+((unsigned char) b)*(1<<8)+((unsigned char) c)*(1<<16)+((unsigned char) d)*(1<<24)) 

const char csfntVersion[4] = { 0x0, 0x1, 0x0, 0x0 };

struct OffsetTable
{
	/*!
	 * According to http://www.microsoft.com/typography/otspec/otff.htm
	 * section "Organization of an OpenType Font" this has to be 0x00010000.
	 * This is the value of the field csfntVersion
	 */
	unsigned char sfntVersion[4];
	unsigned short numTables;
	unsigned short searchRange;
	unsigned short entrySelector;
	unsigned short rangeShift;
};

struct TableDirectory
{
	union {
		unsigned char bytes[4];
		unsigned int uint;
	} tag;
	unsigned int checkSum;
	unsigned int offset;
	unsigned int length;
};

struct cmapTableEntry
{
	unsigned short platformID;
	unsigned short encodingID;
	unsigned int offset;
};

struct cmapTable
{
	unsigned short version;
	unsigned short numTables;
	ArrayBlock<cmapTableEntry> cmapTableEntries;
};

struct glyfTable
{
	/*!
	 * If the number of contours is greater than or equal to zero, 
	 * this is a single glyph; if negative, this is a composite glyph.
	 */
	SHORT	numberOfContours;
	//! Minimum x for coordinate data.
	SHORT	xMin;
	//! Minimum y for coordinate data.
	SHORT	yMin;
	//! Maximum x for coordinate data.
	SHORT	xMax;
	//! Maximum y for coordinate data.
	SHORT	yMax;
};


struct cmapSubTable0
{
	USHORT	format;            //! Format number is set to 0.
	USHORT	length;            //! This is the length in bytes of the subtable.
	USHORT	language;          //! Please see �Note on the language field in 'cmap' subtables� in this document.
	BYTE	glyphIdArray[256]; //! An array that maps character codes to glyph index values.
};

struct cmapSubTable4
{
	USHORT	format;                   //! Format number is set to 4.
	USHORT	length;                   //! This is the length in bytes of the subtable.
	USHORT	language;                 //! Please see �Note on the language field in 'cmap' subtables� in this document.
	USHORT	segCountX2;               //! 2 x segCount.
	USHORT	searchRange;              //! 2 x (2**floor(log2(segCount)))
	USHORT	entrySelector;            //! log2(searchRange/2)
	USHORT	rangeShift;               //! 2 x segCount - searchRange
	//USHORT	endCount[segCount]
	ArrayBlock<USHORT> endCount;      //! End characterCode for each segment, last=0xFFFF.
	USHORT	reservedPad;              //! Set to 0.
	// USHORT	startCount[segCount]
	ArrayBlock<USHORT> startCount;    //! Start character code for each segment.
	// SHORT	idDelta[segCount]
	ArrayBlock<SHORT> idDelta;        //! Delta for all character codes in segment.
	// USHORT	idRangeOffset[segCount]
	ArrayBlock<USHORT> idRangeOffset; //! Offsets into glyphIdArray or 0
	// USHORT	glyphIdArray[ ]
	ArrayBlock<USHORT> glyphIdArray;  //! Glyph index array (arbitrary length)
};

struct cmapSubTable6
{
	USHORT	format;                  //! Format number is set to 6.
	USHORT	length;                  //! This is the length in bytes of the subtable.
	USHORT	language;                //! Please see �Note on the language field in 'cmap' subtables� in this document.
	USHORT	firstCode;               //! First character code of subrange.
	USHORT	entryCount;              //! Number of character codes in subrange.
	// glyphIdArray [entryCount]
	ArrayBlock<USHORT> glyphIdArray; //! Array of glyph index values for character codes in the range.
};


// Stop packing structures to 1-byte boundaries
#pragma pack(pop)



struct TrueTypeFont
{
	ArrayBlock<TableDirectory> tableDirectories;
	cmapTable *mcmapTable;
};

template <typename T> void switchEndianess(T* in_pVar)
{
	T buf=*in_pVar;
	unsigned char* pByteVar=(unsigned char*) in_pVar;
	unsigned char* pByteVarBuf=(unsigned char*) &buf;

	size_t size=sizeof(T);

	for (size_t i=0; i<size; i++)
		pByteVar[i]=pByteVarBuf[size-1-i];
}

template <typename T> T convertEndianess(const T& in_var)
{
	T out=in_var;
	T buf=in_var;
	unsigned char* pOut=(unsigned char*) &out;
	unsigned char* pByteVarBuf=(unsigned char*) &buf;

	size_t size=sizeof(T);

	for (size_t i=0; i<size; i++)
		pOut[i]=pByteVarBuf[size-1-i];

	return out;
}

/*!
 Computes the floor (rounding down if necessary) of the binary logarithm

 if 0 is given 0xFF gets returned
 */
inline unsigned short floorLog2(unsigned short value)
{
	unsigned short out_value = 16;

	for (signed char i=15; i>=0; i--)
	{
		if (value & (1u<<i))
		{
			return i;
		}
	}

	return 0xFF;
}

int readTTF(char* filename);

#endif
