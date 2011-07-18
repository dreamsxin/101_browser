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

#ifndef _JpegDecoder
#define _JpegDecoder

#include "MiniStdlib/MTAx_cstdio.h"
#include "IO/ByteStream.h"
#include "IO/SetjmpStream.h"
#include "JpegDecoder/JpegContext.h"

#ifdef __cplusplus
extern "C" {
#endif

// E.2.1 Control procedure for decoding compressed image data
ReadResult Decode_image(void *in_pStreamState, 
	ByteStreamInterface in_byteStreamReadInterface);
// E.2.2 Control procedure for decoding a frame
void Decode_frame(SetjmpStreamState *in_out_pSetjmpStreamState, 
	ByteStreamInterface in_setjmpStreamReadInterface, 
	unsigned char currentMarker, RestartInterval* in_pri);
// E.2.3 Control procedure for decoding a scan
void Decode_scan(SetjmpStreamState *in_out_pSetjmpStreamState, 
	ByteStreamInterface in_setjmpStreamReadInterface, 
	RestartInterval in_ri);
// E.2.4 Control procedure for decoding a restart interval
void Decode_restart_interval(SetjmpStreamState *in_out_pSetjmpStreamState, 
	ByteStreamInterface in_setjmpStreamReadInterface, 
	RestartInterval in_ri);
void Reset_decoder();
// E.2.5 Control procedure for decoding a minimum coded unit (MCU)
void Decode_MCU();
void Decode_data_unit();

#ifdef __cplusplus
}
#endif

#endif
