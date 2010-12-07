#ifndef _JpegDecoder
#define _JpegDecoder

#include <cstdio>
#include "JpegDecoder/JpegSegments.h"

// E.2.1 Control procedure for decoding compressed image data
void Decode_image(FILE* jpegFile);
// E.2.2 Control procedure for decoding a frame
void Decode_frame(FILE* jpegFile, unsigned char currentMarker, RestartInterval* in_pri);
// E.2.3 Control procedure for decoding a scan
void Decode_scan(FILE* jpegFile, RestartInterval in_ri);
// E.2.4 Control procedure for decoding a restart interval
void Decode_restart_interval(FILE* jpegFile, RestartInterval in_ri);
void Reset_decoder();
// E.2.5 Control procedure for decoding a minimum coded unit (MCU)
void Decode_MCU();
void Decode_data_unit();

#endif
