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

// See  T.081 Table B.1 (page 32) – Marker code assignments

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

// Other markers
#define SOI_MARKER    0xD8
#define EOI_MARKER    0xD9
#define SOS_MARKER    0xDA // Start of scan
#define DQT_MARKER    0xDB // Define quantization table(s)
#define DRI_MARKER    0xDD // Define restart interval
#define APP_0_MARKER  0xE0 // Reserved for application segments
#define APP_1_MARKER  0xE1 // Reserved for application segments
#define APP_2_MARKER  0xE2 // Reserved for application segments
#define APP_3_MARKER  0xE3 // Reserved for application segments
#define APP_4_MARKER  0xE4 // Reserved for application segments
#define APP_5_MARKER  0xE5 // Reserved for application segments
#define APP_6_MARKER  0xE6 // Reserved for application segments
#define APP_7_MARKER  0xE7 // Reserved for application segments
#define APP_8_MARKER  0xE8 // Reserved for application segments
#define APP_9_MARKER  0xE9 // Reserved for application segments
#define APP_10_MARKER 0xEA // Reserved for application segments
#define APP_11_MARKER 0xEB // Reserved for application segments
#define APP_12_MARKER 0xEC // Reserved for application segments
#define APP_13_MARKER 0xED // Reserved for application segments
#define APP_14_MARKER 0xEE // Reserved for application segments
#define APP_15_MARKER 0xEF // Reserved for application segments
#define COM_MARKER    0xFE // Comment





// defined in T.087 section "C.1.1 Marker assignments"
#define SOF_55_MARKER 0xF7
#define LSE_MARKER    0xF8

#endif
