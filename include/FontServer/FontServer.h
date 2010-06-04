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

struct Table_head
{
	Fixed	tableVersionNumber;   // 0x00010000 for version 1.0.
	Fixed	fontRevision;         // Set by font manufacturer.
	UINT	checkSumAdjustment;   // To compute: set it to 0, sum the entire font as ULONG, then store 0xB1B0AFBA - sum.
	UINT	magicNumber;          // Set to 0x5F0F3CF5.
	USHORT	flags;                // Bit 0: Baseline for font at y=0;
	                              // Bit 1: Left sidebearing point at x=0;
	                              // Bit 2: Instructions may depend on point size; 
	                              // Bit 3: Force ppem to integer values for all internal scaler math; may use fractional ppem sizes if this bit is clear; 
	                              // Bit 4: Instructions may alter advance width (the advance widths might not scale linearly); 
	                              // Bits 5-10: These should be set according to Apple's specification . However, they are not implemented in OpenType. 
	                              // Bit 11: Font data is 'lossless,' as a result of having been compressed and decompressed with the Agfa MicroType Express engine.
	                              // Bit 12: Font converted (produce compatible metrics)
	                              // Bit 13: Font optimized for ClearType™. Note, fonts that rely on embedded bitmaps (EBDT) for 
	                              //         rendering should not be considered optimized for 
	                              //         ClearType, and therefore should keep this bit cleared.
	                              // Bit 14: Reserved, set to 0
	                              // Bit 15: Reserved, set to 0 
	USHORT	unitsPerEm;           // Valid range is from 16 to 16384. This value should be a power of 2 for fonts that have TrueType outlines.
	LONGDATETIME	created;      // Number of seconds since 12:00 midnight, January 1, 1904. 64-bit integer
	LONGDATETIME	modified;     // Number of seconds since 12:00 midnight, January 1, 1904. 64-bit integer
	SHORT	xMin;                 // For all glyph bounding boxes.
	SHORT	yMin;                 // For all glyph bounding boxes.
	SHORT	xMax;                 // For all glyph bounding boxes.
	SHORT	yMax;                 // For all glyph bounding boxes.
	USHORT	macStyle;             // Bit 0: Bold (if set to 1); 
		                          // Bit 1: Italic (if set to 1) 
		                          // Bit 2: Underline (if set to 1) 
		                          // Bit 3: Outline (if set to 1) 
		                          // Bit 4: Shadow (if set to 1) 
		                          // Bit 5: Condensed (if set to 1) 
		                          // Bit 6: Extended (if set to 1) 
		                          // Bits 7-15: Reserved (set to 0).
	USHORT	lowestRecPPEM;        // Smallest readable size in pixels.
	SHORT	fontDirectionHint;    // Deprecated (Set to 2). 
	                              // 0: Fully mixed directional glyphs; 
	                              // 1: Only strongly left to right; 
	                              // 2: Like 1 but also contains neutrals; 
	                              // -1: Only strongly right to left; 
	                              // -2: Like -1 but also contains neutrals 
		                          // (A neutral character has no inherent directionality; 
		                          // it is not a character with zero (0) width. Spaces and 
		                          // punctuation are examples of neutral characters. 
		                          // Non-neutral characters are those with inherent 
		                          // directionality. For example, Roman letters (left-to-right) 
		                          // and Arabic letters(right-to-left) have directionality. 
		                          // In a “normal” Roman font where spaces and punctuation 
		                          // are present, the font direction hints should be 
		                          // set to two (2)).
	SHORT	indexToLocFormat;     // 0 for short offsets, 1 for long.
	SHORT	glyphDataFormat;      // 0 for current format.
};

struct Table_maxp
{
	Fixed tableVersionNumber;      // 0x00005000 for version 0.5
	                               // (Note the difference in the 
	                               // representation of a non-zero fractional part, 
	                               // in Fixed numbers.)
	                               // 0x00010000 for version 1.0.
	USHORT	numGlyphs;             // The number of glyphs in the font.

	//! The following fields are only defined if tableVersionNumber == 0x00010000
	USHORT	maxPoints;             // Maximum points in a non-composite glyph.
	USHORT	maxContours;           // Maximum contours in a non-composite glyph.
	USHORT	maxCompositePoints;    // Maximum points in a composite glyph.
	USHORT	maxCompositeContours;  // Maximum contours in a composite glyph.
	USHORT	maxZones;              // 1 if instructions do not use the twilight zone (Z0), or 2 if instructions do use Z0; should be set to 2 in most cases.
	USHORT	maxTwilightPoints;     // Maximum points used in Z0.
	USHORT	maxStorage;            // Number of Storage Area locations.
	USHORT	maxFunctionDefs;       // Number of FDEFs.
	USHORT	maxInstructionDefs;    // Number of IDEFs.
	USHORT	maxStackElements;      // Maximum stack depth (This includes Font and CVT Programs, as well as the instructions for each glyph).
	USHORT	maxSizeOfInstructions; // Maximum byte count for glyph instructions.
	USHORT	maxComponentElements;  // Maximum number of components referenced at “top level” for any composite glyph.
	USHORT	maxComponentDepth;     // Maximum levels of recursion; 1 for simple components.
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

TableRecord* getTableRecordPointer(FILE* fontFile, const ArrayBlock<TableRecord>* in_pTableDirectory, UINT in_tag);

bool readTable_cmap(FILE* fontFile, TrueTypeFont* in_trueTypeFont);
bool readTable_glyf(FILE* fontFile, TrueTypeFont* in_trueTypeFont);
bool readTable_head(FILE* fontFile, TrueTypeFont* in_trueTypeFont, Table_head* lpTable_head);
bool readTable_loca(FILE* fontFile, TrueTypeFont* in_trueTypeFont);
bool readTable_maxp(FILE* fontFile, TrueTypeFont* in_trueTypeFont, Table_maxp* in_pTable_maxp);

#endif
