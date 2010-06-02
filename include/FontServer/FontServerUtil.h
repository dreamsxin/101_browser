#ifndef _FontServerUtil_h
#define _FontServerUtil_h

// TODO: Move into BasicDataStructures

template <typename T> void switchEndianess(T* in_pVar)
{
	T buf=*in_pVar;
	unsigned char* pByteVar=(unsigned char*) in_pVar;
	unsigned char* pByteVarBuf=(unsigned char*) &buf;

	size_t size=sizeof(T);

	for (size_t i=0; i<size; i++)
		pByteVar[i]=pByteVarBuf[size-1-i];
}

template <typename T> T convertEndianess(const T& in_var)
{
	T out=in_var;
	T buf=in_var;
	unsigned char* pOut=(unsigned char*) &out;
	unsigned char* pByteVarBuf=(unsigned char*) &buf;

	size_t size=sizeof(T);

	for (size_t i=0; i<size; i++)
		pOut[i]=pByteVarBuf[size-1-i];

	return out;
}

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
