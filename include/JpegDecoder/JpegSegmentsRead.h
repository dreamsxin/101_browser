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

#ifndef _JpegSegmentsRead
#define _JpegSegmentsRead

#include "JpegDecoder/T81_SectionB2_B3.h"
#include "JpegDecoder/JpegDecoderUtil.h"
#include "JpegDecoder/JpegContext.h"

#ifdef __cplusplus
extern "C" {
#endif

void readFrameHeader(SetjmpStreamState *in_out_pSetjmpStreamState, 
	ByteStreamInterface in_setjmpStreamReadInterface, 
	FrameHeader* in_pFrameHeader, uint8_t in_SOF_n);

void readScanHeader(SetjmpStreamState *in_out_pSetjmpStreamState, 
	ByteStreamInterface in_setjmpStreamReadInterface, 
	ScanHeader* in_pScanHeader);

void readRestartInterval(SetjmpStreamState *in_out_pSetjmpStreamState, 
	ByteStreamInterface in_setjmpStreamReadInterface, 
	RestartIntervalState* in_pRestartIntervalState);

#ifdef __cplusplus
}
#endif

#endif
