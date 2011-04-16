#include "RFC1950/RFC1950_zlib.h"

void parseRFC1950(void *in_pStreamState, ByteStreamReadInterface in_readStream, bool in_supportPresetDictionary)
{
	ZlibStreamHeader header;
	uint16_t LZ77_window_size;
	uint32_t DICTID;

	if ((*in_readStream.pRead)(in_pStreamState, &header, sizeof(header)) != sizeof(header))
	{
		// TODO - error
	}

	/*
	 * RFC 1950, section 2.2. Data format:
	 *
	 * "CM = 8
     * denotes the "deflate" compression method with a window size up
     * to 32K.  This is the method used by gzip and PNG (see
     * references [1] and [2] in Chapter 3, below, for the reference
     * documents)."
	 */
	if (header.CMF.fields.CM != 8)
	{
		// TODO - error
	}

	/*
	 * RFC 1950, section 2.2 Data format:
	 *
	 * "Values
     * of CINFO above 7 are not allowed in this version of the
     * specification."
	 */
	// CND:RFC1950_zlib_c:29
	if (header.CMF.fields.CINFO > 7)
	{
		// TODO - error
	}

	/*
	 * RFC 1950, section 2.2 Data format:
	 *
	 * "CINFO (Compression info)
	 * For CM = 8 [because of CND:RFC1950_zlib_c:22], 
	 * CINFO is the base-2 logarithm of the LZ77 window
	 * size, minus eight (CINFO=7 indicates a 32K window size)."
	 *
	 * Note that because of CND:RFC1950_zlib_c:29 16 bits are
	 * sufficient for LZ77_window_size.
	 */
	LZ77_window_size = 1<<(header.CMF.fields.CINFO+8);

	/*
	 * RFC 1950, section 2.2 Data format:
	 *
	 * "The FCHECK value must be such that CMF and FLG, when viewed as
	 * a 16-bit unsigned integer stored in MSB order (CMF*256 + FLG),
	 * is a multiple of 31."
	 */
	if ((header.CMF.byteValue * 256 + header.FLG.byteValue) % 31 != 0)
	{
		// TODO - error
	}

	if (!in_supportPresetDictionary && header.FLG.fields.FDICT)
	{
		// TODO - error
	}

	if (header.FLG.fields.FDICT)
	{
		if ((*in_readStream.pRead)(in_pStreamState, &DICTID, sizeof(DICTID)) != sizeof(DICTID))
		{
			// TODO - error
		}
	}
}
