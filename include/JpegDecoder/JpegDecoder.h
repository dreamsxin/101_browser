#ifndef _JpegDecoder
#define _JpegDecoder

#include <cstdio>
#include "JpegDecoder/JpegSegments.h"

void Decode_image(FILE* jpegFile);
void Decode_frame(FILE* jpegFile, unsigned char currentMarker, RestartInterval* in_pri);
void Decode_scan(FILE* jpegFile, RestartInterval in_ri);
void Decode_restart_interval(FILE* jpegFile, RestartInterval in_ri);
void Reset_decoder();
void Decode_MCU();
void Decode_data_unit();

#endif
