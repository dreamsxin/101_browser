#ifndef _MTAx_RFC1950_zlib_h
#define _MTAx_RFC1950_zlib_h

#include "Util/ReadResult.h"
#include "MiniStdlib/MTAx_cstdio.h"
#include "MiniStdlib/cstdint.h"
//#include "MiniStdlib/cstdbool.h"

#pragma pack(push, 1)

/*!
 * Compare section
 * 2.2. Data format
 * of RFC 1950
 */
typedef struct
{
	struct {
		uint8_t CM : 4;
		uint8_t CINFO : 4;
	} CMF;
	struct {
		uint8_t FCHECK : 5;
		uint8_t FDICT : 1;
		uint8_t FLEVEL : 2;
	} FLG;
} ZlibStreamHeader;

#pragma pack(pop)



#endif
