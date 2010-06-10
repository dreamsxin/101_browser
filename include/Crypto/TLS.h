#ifndef _TLS_h
#define _TLS_h

#include "BasicDataStructures/Types.h"

namespace MTAx
{
	namespace TLS
	{
		const uint8_t ContentType_handshake = 22;

		// Pack structures to 1-byte boundaries
#pragma pack(push, 1)
		struct ProtocolVersion {
			uint8_t major;
			uint8_t minor;
		};


		struct TLSPlaintextHeader {
			uint8_t type;
			ProtocolVersion version;
			uint16_t length;
		};
#pragma pack(pop)

		const ProtocolVersion TLS_1_2_ProtocolVersion = { 3, 3 };
	}

}

#endif
