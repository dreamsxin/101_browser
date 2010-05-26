#ifndef _FontServer_h
#define _FontServer_h

#include "BasicDataStructures/Types.h"
#include <vector>
#include "BasicDataStructures/Memory/ArrayBlock.h"

// Pack structures to 1-byte boundaries
#pragma pack(push, 1)

#define CHAR4_TO_UINT_LIL_ENDIAN(a, b, c, d) (((unsigned char) a)+((unsigned char) b)*(1<<8)+((unsigned char) c)*(1<<16)+((unsigned char) d)*(1<<24)) 

const char csfntVersion[4] = { 0x0, 0x1, 0x0, 0x0 };

class OffsetTable
{
public:
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

class TableDirectory
{
public:
	union {
		unsigned char bytes[4];
		unsigned int uint;
	} tag;
	unsigned int checkSum;
	unsigned int offset;
	unsigned int length;
};

class cmapTableEntry
{
public:
	unsigned short platformID;
	unsigned short encodingID;
	unsigned int offset;
};

class cmapTable
{
public:
	unsigned short version;
	unsigned short numTables;
	std::vector<cmapTableEntry> cmapTableEntries;
};

struct cmapSubTable0
{
	USHORT	format;            // Format number is set to 0.
	USHORT	length;            // This is the length in bytes of the subtable.
	USHORT	language;          // Please see "Note on the language field in 'cmap' subtables" in this document.
	BYTE	glyphIdArray[256]; // An array that maps character codes to glyph index values.
};

struct cmapSubTable6
{
	USHORT	format;                  // Format number is set to 6.
	USHORT	length;                  // This is the length in bytes of the subtable.
	USHORT	language;                // Please see "Note on the language field in 'cmap' subtables" in this document.
	USHORT	firstCode;               // First character code of subrange.
	USHORT	entryCount;              // Number of character codes in subrange.
	// glyphIdArray [entryCount]
	ArrayBlock<USHORT> glyphIdArray; // Array of glyph index values for character codes in the range.
};


// Stop packing structures to 1-byte boundaries
#pragma pack(pop)



class TrueTypeFont
{
public:
	OffsetTable offsetTable;
	std::vector<TableDirectory> tableDirectories;

	// Required
	void* table_OS2;  // 'OS/2'
	void* table_cmap; // 'cmap' - mapping of characters to glyphs
	void* table_glyf; // 'glyf'
	void* table_head; // 'head'
	void* table_hhea; // 'hhea'
	void* table_hmtx; // 'hmtx'
	void* table_loca; // 'loca'
	void* table_maxp; // 'maxp'
	void* table_name; // 'name'
	void* table_post; // 'post'

	// Optional
	void* table_DSIG; // 'DSIG' - meaning unknown -> 'digital signature'?
	void* table_GDEF; // 'GDEF' - for Open Type
	void* table_GPOS; // 'GPOS' - for Open Type
	void* table_LTSH; // 'LTSH'
	void* table_PCLT; // 'PCLT' - PCL5 
	void* table_VDMX; // 'VDMX'
	void* table_cvt;  // 'cvt '
	void* table_fpgm; // 'fpgm'
	void* table_gasp; // 'gasp'
	void* table_hdmx; // 'hdmx'
	void* table_kern; // 'kern' - kerning
	void* table_prep; // 'prep'
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
