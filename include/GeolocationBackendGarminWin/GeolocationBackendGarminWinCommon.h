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

#ifndef _GeolocationBackendGarminWinCommon_h
#define _GeolocationBackendGarminWinCommon_h

typedef enum
{
	ReceiveByDeviceIoControl,
	ReceiveByReadFile
} ReceivePacketState;


typedef enum
{
	/*
	 * ReceivePacketResultNoPacket is returned when no additional packet is received. 
	 * This means as a corollary that we can start sending packages.
	 */
	ReceivePacketResultNoPacket,
	/*
	 * ReceivePacketResultPacket is returned when a packet is received after which we 
	 * can send packages.
	 */
	ReceivePacketResultPacket,
	/*
	 * ReceivePacketResultPacketContinueRead
	 * is returned if we have to continue reading
	 * before sending another packet
	 */
	ReceivePacketResultPacketContinueRead,

	ReceivePacketResultErrorAllocation,
	ReceivePacketResultErrorReadFile,
	ReceivePacketResultErrorInvalidData,
	ReceivePacketResultErrorInvalidState
} ReceivePacketResult;

#endif
