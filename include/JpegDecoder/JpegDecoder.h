#ifndef _JpegDecoder
#define _JpegDecoder

#include <cstdio>

void Decode_image(FILE* jpegFile);
void Decode_frame(FILE* jpegFile, unsigned char currentMarker);
void Decode_scan(FILE* jpegFile);
void Decode_restart_interval();

#endif
