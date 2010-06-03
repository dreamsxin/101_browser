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

struct TableRecord
{
	union {
		unsigned char bytes[4];
		UINT uint;
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

struct Table_cmap
{
	unsigned short version;
	unsigned short numTables;
	ArrayBlock<cmapTableEntry> cmapTableEntries;
};

struct Table_glyf
{
	SHORT	numberOfContours;        /*!
	                                  * If the number of contours is greater than or 
									  * equal to zero, this is a single glyph; 
									  * if negative, this is a composite glyph.
	                                  */
	SHORT	xMin;                   //! Minimum x for coordinate data.
	SHORT	yMin;                   //! Minimum y for coordinate data.
	SHORT	xMax;                   //! Maximum x for coordinate data.
	SHORT	yMax;                   //! Maximum y for coordinate data.
};


struct cmapSubTable0
{
	USHORT	format;            //! Format number is set to 0.
	USHORT	length;            //! This is the length in bytes of the subtable.
	USHORT	language;          //! Please see “Note on the language field in 'cmap' subtables“ in this document.
	BYTE	glyphIdArray[256]; //! An array that maps character codes to glyph index values.
};

struct cmapSubTable4
{
	USHORT	format;                   //! Format number is set to 4.
	USHORT	length;                   //! This is the length in bytes of the subtable.
	USHORT	language;                 //! Please see “Note on the language field in 'cmap' subtables“ in this document.
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
	USHORT	language;                //! Please see “Note on the language field in 'cmap' subtables“ in this document.
	USHORT	firstCode;               //! First character code of subrange.
	USHORT	entryCount;              //! Number of character codes in subrange.
	// glyphIdArray [entryCount]
	ArrayBlock<USHORT> glyphIdArray; //! Array of glyph index values for character codes in the range.
};


// Stop packing structures to 1-byte boundaries
#pragma pack(pop)


struct TrueTypeFont
{
	ArrayBlock<TableRecord> tableDirectory;
	Table_cmap *mpTable_cmap;
	Table_glyf *mpTable_glyf;
};


int readTTF(char* filename);
bool readTableRecord(FILE* fontFile, TableRecord* in_pTableRecord);

/*!
 * Returns true if 
 * a.) the checksum of the table by in_pTableRecord is correct.
 * and
 * b.) the position in fontFile could successfully become saved and restored
 * 
 * Otherwise returns false.
 *
 * Invariants:
 * I_verifyCheckSum_1: if we return true the position in fontFile is not changed
 *                     (but it may change if false is returned)
 */
bool verifyCheckSum(FILE* fontFile, TableRecord* in_pTableRecord);
bool readOffsetTable(FILE* fontFile, OffsetTable* in_pOffsetTable);

bool readTable(FILE* fontFile, const ArrayBlock<TableRecord>* in_pTableDirectory, UINT in_tag);

bool readTable_cmap(FILE* fontFile, TableRecord* in_pTableRecord);
bool readTable_glyf(FILE* fontFile, TableRecord* in_pTableRecord);
bool readTable_loca(FILE* fontFile, TableRecord* in_pTableRecord);

#endif
