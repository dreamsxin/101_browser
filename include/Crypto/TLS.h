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

#ifndef _TLS_h
#define _TLS_h

#include "BasicDataStructures/Types.h"

namespace MTAx
{
	namespace TLS
	{
		typedef uint8_t ContentType;
		const ContentType ContentType_handshake = 22;

		// Pack structures to 1-byte boundaries
#pragma pack(push, 1)
		typedef struct {
			uint8_t major;
			uint8_t minor;
		} ProtocolVersion;

		const ProtocolVersion TLS_1_2_ProtocolVersion = { 3, 3 };

		typedef uint8_t HandshakeType;
		HandshakeType HandshakeType_client_hello = 1;

		typedef struct {
			uint32_t gmt_unix_time;
			uint8_t random_bytes[28];
		} Random;



		typedef struct {
			ContentType type;
			ProtocolVersion version;
			uint16_t length;
		} TLSPlaintextHeader;

		struct {
			HandshakeType msg_type;          /* handshake type */
			uint8_t length[3];         /* bytes in message */
		} HandshakeHeader;


		struct {
			ProtocolVersion client_version;
			Random random;
			//SessionID session_id;
			//CipherSuite cipher_suites<2..2^16-2>;
			//CompressionMethod compression_methods<1..2^8-1>;
		} ClientHello;
#pragma pack(pop)

	}

}

#endif
