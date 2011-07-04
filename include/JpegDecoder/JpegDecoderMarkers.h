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

#ifndef _JpegDecoderMarkers
#define _JpegDecoderMarkers

// See  T.081 Table B.1 – Marker code assignments

// Start Of Frame markers, non-differential, Huffman coding
#define SOF_0_MARKER  0xC0 // Baseline DCT
#define SOF_1_MARKER  0xC1 // Extended sequential DCT
#define SOF_2_MARKER  0xC2 // Progressive DCT
#define SOF_3_MARKER  0xC3 // Lossless (sequential)



#define DHT_MARKER    0xC4
// #define JPG_MARKER 0xC8 // Reserved for JPEG extensions
#define SOF_9_MARKER  0xC9 // Extended sequential DCT
#define SOF_10_MARKER 0xCA // Progressive DCT
#define SOF_11_MARKER 0xCB // Lossless (sequential)


#define DAC_MARKER    0xCC
#define SOF_15_MARKER 0xCF

// defined in T.087 section "C.1.1 Marker assignments"
#define SOF_55_MARKER 0xF7
#define LSE_MARKER    0xF8


#define SOI_MARKER    0xD8
#define EOI_MARKER    0xD9

#define SOS_MARKER    0xDA
#define DRI_MARKER    0xDD

#endif
