/*
 * Copyright 2008-2011 Wolfgang Keller
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _GIF_h
#define _GIF_h

#include "Util/ReadResult.h"
#include "MiniStdlib/cstdint.h"
#include "MiniStdlib/cstdbool.h"
#include "MiniStdlib/declspec.h"
#include "IO/SetjmpStream.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push, 1)

#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable : 4214)
#endif

typedef struct
{
	uint16_t Logical_Screen_Width;
	uint16_t Logical_Screen_Height;
	uint8_t Size_Of_Global_Color_Table : 3;
	uint8_t Sort_Flag : 1;
	uint8_t Color_Resolution : 3;
	uint8_t Global_Color_Table_Flag : 1;
	uint8_t Background_Color_Index;
	uint8_t Pixel_Aspect_Ratio;
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

#ifdef _WIN32
#pragma warning(pop)
#endif


typedef struct
{
	uint8_t r, g, b;
} Rgb8Color;

typedef struct
{
	Logical_Screen_Descriptor logicalScreenDescriptor;
	Rgb8Color *globalColorTable;
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

typedef struct
{
	uint8_t Block_Size;
	uint8_t *Data_Values;
} Data_SubBlock;

typedef struct
{
	uint8_t Block_Size;
	char Application_Identifier[8];
	char Application_Authentication_Code[3];
} Application_Extension;

typedef struct
{
	Image_Descriptor imageDescriptor;
	Rgb8Color *localColorTable;
	uint8_t *imageData;
} TableBased_Image;

#pragma pack(pop)

// Helper functions
uint16_t colorsOfColorTable(uint8_t in_sizeOfColorTable);
uint16_t bytesOfColorTable(uint8_t in_sizeOfColorTable);

// Grammar words
DLLEXPORT ReadResult read_GIF_Data_Stream(void *in_pStreamState, 
	ByteStreamInterface in_byteStreamReadInterface, 
	GIF_Data_Stream *in_pDataStream);
void read_Logical_Screen(SetjmpStreamState *in_out_pSetjmpStreamState, 
	ByteStreamInterface in_byteStreamReadInterface, 
	Logical_Screen *in_pLogicalScreen, bool in_is89a);
void read_Data(SetjmpStreamState *in_out_pSetjmpStreamState, 
	ByteStreamInterface in_byteStreamReadInterface, 
	uint8_t in_introducer, bool in_is89a, 
	const Logical_Screen *in_cpLogicalScreen);
/*
* Preconditions:
* PRE:GIF_h_126: (0x21 == in_separator) || (0x2C == in_separator)
* PRE:GIF_h_127: (0x21 == in_separator) => ((in_label == 0xF9) || (in_label == 0x01))
* PRE:GIF_h_128: (0x21 == in_separator) => we have a GIF 89a file
*/
void read_Graphic_Block(SetjmpStreamState *in_out_pSetjmpStreamState, 
	ByteStreamInterface in_byteStreamReadInterface, 
	uint8_t in_separator, uint8_t in_label, bool in_is89a, 
	const Logical_Screen *in_cpLogicalScreen);
void read_GraphicRendering_Block(SetjmpStreamState *in_out_pSetjmpStreamState, 
	ByteStreamInterface in_byteStreamReadInterface, 
	uint8_t in_separator, uint8_t in_label, bool in_is89a, 
	const Logical_Screen *in_cpLogicalScreen);
void read_TableBased_Image(SetjmpStreamState *in_out_pSetjmpStreamState, 
	ByteStreamInterface in_byteStreamReadInterface, 
	TableBased_Image *in_pTableBasedImage, bool in_is89a, 
	const Logical_Screen *in_cpLogicalScreen);
/*
* Precondition:
* PRE:GIF_h_129: we have a GIF 89a file
*/
void read_SpecialPurpose_Block(SetjmpStreamState *in_out_pSetjmpStreamState, 
	ByteStreamInterface in_byteStreamReadInterface, 
	uint8_t in_label);

// Terminal symbols
void read_Header(SetjmpStreamState *in_out_pSetjmpStreamState, 
	ByteStreamInterface in_byteStreamReadInterface, 
	Header *in_pHeader, bool *out_pIs89a);
void read_Trailer(SetjmpStreamState *in_out_pSetjmpStreamState, 
	ByteStreamInterface in_byteStreamReadInterface);
void read_Logical_Screen_Descriptor(SetjmpStreamState *in_out_pSetjmpStreamState, 
	ByteStreamInterface in_byteStreamReadInterface);
#if 0
void read_Global_Color_Table(void *in_pStreamState, 
	ByteStreamInterface in_byteStreamReadInterface);
#endif
void read_Graphic_Control_Extension(SetjmpStreamState *in_out_pSetjmpStreamState, 
	ByteStreamInterface in_byteStreamReadInterface);

void read_Plain_Text_Extension(SetjmpStreamState *in_out_pSetjmpStreamState, 
	ByteStreamInterface in_byteStreamReadInterface);

void read_Image_Descriptor(SetjmpStreamState *in_out_pSetjmpStreamState, 
	ByteStreamInterface in_byteStreamReadInterface, 
	Image_Descriptor* out_pImageDescriptor, bool in_is89a, 
	const Logical_Screen_Descriptor *in_cpLogicalScreenDescriptor);
#if 0
void read_Local_Color_Table(SetjmpStreamState *in_out_pSetjmpStreamState, 
	ByteStreamInterface in_byteStreamReadInterface);
#endif
void read_Image_Data(SetjmpStreamState *in_out_pSetjmpStreamState, 
	ByteStreamInterface in_byteStreamReadInterface, 
	const Image_Descriptor *in_cpImageDescriptor, 
	const Rgb8Color *in_pColorTable, 
	uint16_t in_colorTableSize);
/*
* Precondition:
* PRE:GIF_h_148: we have a GIF 89a file
*/
void read_Application_Extension(SetjmpStreamState *in_out_pSetjmpStreamState, 
	ByteStreamInterface in_byteStreamReadInterface);
/*
* Precondition:
* PRE:GIF_h_158: we have a GIF 89a file
*/
void read_Comment_Extension(SetjmpStreamState *in_out_pSetjmpStreamState, 
	ByteStreamInterface in_byteStreamReadInterface);

void skipBlock(SetjmpStreamState *in_out_pSetjmpStreamState, 
	ByteStreamInterface in_byteStreamReadInterface);

#ifdef __cplusplus
}
#endif

#endif
