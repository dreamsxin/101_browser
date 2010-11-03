#ifndef _GIF_h
#define _GIF_h

#include "MiniStdlib/MTAx_cstdio.h"
#include "MiniStdlib/cstdint.h"
#include "MiniStdlib/cstdbool.h"

#pragma pack(push, 1)

typedef struct
{
	unsigned short Logical_Screen_Width;
	unsigned short Logical_Screen_Height;
	unsigned char Size_Of_Global_Color_Table : 3;
	unsigned char Sort_Flag : 1;
	unsigned char Color_Resolution : 3;
	unsigned char Global_Color_Table_Flag : 1;
    unsigned char Background_Color_Index;
    unsigned char Pixel_Aspect_Ratio;
} Logical_Screen_Descriptor;

typedef struct
{
	uint8_t Image_Separator;
	uint16_t Image_Left_Position;
	uint16_t Image_Top_Position;
	uint16_t Image_Width;
	uint16_t Image_Height;
	uint8_t Size_of_Local_Color_Table : 3;
	uint8_t Reserved : 2;
	uint8_t Sort_Flag : 1;
	uint8_t Interlace_Flag : 1;
	uint8_t Local_Color_Table_Flag : 1;
} Image_Descriptor;

typedef struct
{
	uint8_t Block_Size;
	uint8_t Transparent_Color_Flag : 1;
	uint8_t User_Input_Flag : 1;
	uint8_t Disposal_Method : 3;
	uint8_t Reserved : 3;
	uint16_t Delay_Time;
	uint8_t Transparent_Color_Index;
} Graphic_Control_Extension;

typedef struct
{
	Logical_Screen_Descriptor logicalScreenDescriptor;
	uint8_t *globalColorTable;
} Logical_Screen;

typedef struct
{
	char Signature[3];
	char Version[3];
} Header;

typedef struct
{
	Header header;
	Logical_Screen logicalScreen;
} GIF_Data_Stream;

#pragma pack(pop)

typedef enum
{
	ReadResultOK,
	ReadResultPrematureEndOfStream,
	ReadResultInvalidData,
	/*
	 * ReadResultInvalidVersion is returned when it could perhaps
	 * be correct, but the claimed version of the file format is
	 * too old
	 */
	ReadResultInvalidVersion,
	ReadResultAllocationFailure
} ReadResult;

#ifdef __cplusplus
extern "C" {
#endif

// Helper functions
size_t bytesOfColorTable(unsigned char in_sizeOfColorTable);

// Grammar words
ReadResult read_GIF_Data_Stream(FILE* in_gifFile, GIF_Data_Stream *in_pDataStream);
ReadResult read_Logical_Screen(FILE* in_gifFile, Logical_Screen *in_pLogicalScreen);
ReadResult read_Data(FILE* in_gifFile, bool in_is89a);
ReadResult read_Graphic_Block(FILE* in_gifFile);
ReadResult read_GraphicRendering_Block(FILE* in_gifFile);
ReadResult read_TableBased_Image(FILE* in_gifFile);
ReadResult read_SpecialPurpose_Block(FILE* in_gifFile);

// Terminal symbols
ReadResult read_Header(FILE* in_gifFile, Header *in_pHeader, bool *out_pIs89a);
ReadResult read_Trailer(FILE* in_gifFile);
ReadResult read_Logical_Screen_Descriptor(FILE* in_gifFile);
ReadResult read_Global_Color_Table(FILE* in_gifFile);
ReadResult read_Graphic_Control_Extension(FILE* in_gifFile, bool in_is89a);
ReadResult read_Plain_Text_Extension(FILE* in_gifFile);
ReadResult read_Image_Descriptor(FILE* in_gifFile, Image_Descriptor* in_pImageDescriptor);
ReadResult read_Local_Color_Table(FILE* in_gifFile);
ReadResult read_Image_Data(FILE* in_gifFile);
ReadResult read_Application_Extension(FILE* in_gifFile);
ReadResult read_Comment_Extension(FILE* in_gifFile);
#ifdef __cplusplus
}
#endif

#endif
