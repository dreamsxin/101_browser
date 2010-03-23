#ifndef _JpegDecoderUtil
#define _JpegDecoderUtil

#include <cstdio>

bool isStandaloneMarker(unsigned char in_marker);
unsigned short flipBytes(unsigned short in_value);
void printSegmentName(unsigned char in_marker);
void printMarkerInformation(unsigned char currentMarker);
unsigned char readMarker(FILE* jpegFile);
void defaultMarkerInterpreter(FILE* jpegFile, unsigned char currentMarker);

#endif