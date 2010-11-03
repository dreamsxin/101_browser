#ifndef _GIF_h
#define _GIF_h

#include "MiniStdlib/MTAx_cstdio.h"

// Grammar words
void read_GIF_Data_Stream(FILE* in_gifFile);
void read_Logical_Screen(FILE* in_gifFile);
void read_Data(FILE* in_gifFile);
void read_Graphic_Block(FILE* in_gifFile);
void read_GraphicRendering_Block(FILE* in_gifFile);
void read_TableBased_Image(FILE* in_gifFile);
void read_SpecialPurpose_Block(FILE* in_gifFile);

// Terminal symbols
void read_Header(FILE* in_gifFile);
void read_Trailer(FILE* in_gifFile);
void read_Logical_Screen_Descriptor(FILE* in_gifFile);
void read_Global_Color_Table(FILE* in_gifFile);
void read_Graphic_Control_Extension(FILE* in_gifFile);
void read_Plain_Text_Extension(FILE* in_gifFile);
void read_Image_Descriptor(FILE* in_gifFile);
void read_Local_Color_Table(FILE* in_gifFile);
void read_Image_Data(FILE* in_gifFile);
void read_Application_Extension(FILE* in_gifFile);
void read_Comment_Extension(FILE* in_gifFile);

#endif
