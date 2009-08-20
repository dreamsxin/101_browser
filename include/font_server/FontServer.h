#ifndef _FontServer_h
#define _FontServer_h

#include "font_server/DynamicMemory.h"
#include <vector>

// Pack structures to 1-byte boundaries
#pragma pack(push, 1)

class OffsetTable
{
public:
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

class CmapTableEntry
{
public:
	unsigned short platformID;
	unsigned short encodingID;
	unsigned int offset;
};

class CmapTable
{
public:
	unsigned short versionNumber;
	unsigned short encodingTablesCount;
	std::vector<CmapTableEntry> cmapTableEntries;
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
	void* table_cmap; // 'cmap'
	void* table_glyf; // 'glyf'
	void* table_head; // 'head'
	void* table_hhea; // 'hhea'
	void* table_hmtx; // 'hmtx'
	void* table_loca; // 'loca'
	void* table_maxp; // 'maxp'
	void* table_name; // 'name'
	void* table_post; // 'post'

	// Optional
	void* table_LTSH; // 'LTSH'
	void* table_cvt;  // 'cvt '
	void* table_fpgm; // 'fpgm'
	void* table_hdmx; // 'hdmx'
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
