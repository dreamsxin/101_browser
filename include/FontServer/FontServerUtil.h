#ifndef _FontServerUtil_h
#define _FontServerUtil_h

// TODO: Move into BasicDataStructures

/*!
 * Computes the floor (rounding down if necessary) of the binary logarithm
 * 
 * If 0 is given we return 0xFF 
 */
inline unsigned short floorLog2(unsigned short value)
{
	unsigned short out_value = 16;

	for (signed char i=15; i>=0; i--)
	{
		if (value & (1u<<i))
		{
			return i;
		}
	}

	return 0xFF;
}

#endif
