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

#include "GeolocationBackendGarminWin/GeolocationGarmin.h"

void sendPacket(HANDLE in_garminHandle, void *in_pPacket, WORD in_usbPacketSize)
{
	DWORD theBytesToWrite = sizeof(Packet_t) - 1 + ((Packet_t *) in_pPacket)->mDataSize;
	DWORD theBytesReturned = 0;

	WriteFile(in_garminHandle, in_pPacket, theBytesToWrite, &theBytesReturned, NULL);

	/*
	 * If the packet size was an exact multiple of the USB packet
	 * size, we must make a final write call with no data.
	 */
	if (theBytesToWrite % in_usbPacketSize == 0)
	{
		WriteFile(in_garminHandle, 0, 0, &theBytesReturned, NULL);
	}
}
