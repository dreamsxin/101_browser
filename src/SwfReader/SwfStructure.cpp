#include "SwfReader/SwfStructure.h"

/*!
 * Reads the number of bits in the bitvalue
 *
 * n tells the number of bits to consider (at the moment it has to be <=8)
 * firstByte is the first byte of the bit value
 *
 * Return value: the number of bits to consider
 */
unsigned char getNumberOfBitsInBitValue(unsigned int in_n, unsigned char in_firstByte)
{
	return in_firstByte >> (8-in_n);
}

bool readRECT(BasicReadStream* in_pStream, RECT* in_pRect)
{
	unsigned char rectFirstByte;
	if (in_pStream->read(&rectFirstByte, 1, 1) != 1)
		return false;

	in_pRect->NBits=getNumberOfBitsInBitValue(5, rectFirstByte);

	// *4: there are 4 N-bit values
	// +5: we also want to store the first 5 bits
	// +7: maximally 7 padding bits may be added
	// /8: we want it in bytes, not bits
	unsigned char numberRectBytes = (in_pRect->NBits*4+5+7)/8;

	if (in_pRect->XminXmaxYminYmax)
	{
		delete[] in_pRect->XminXmaxYminYmax;
		in_pRect->XminXmaxYminYmax = NULL;
	}

	in_pRect->XminXmaxYminYmax = new unsigned char[numberRectBytes];

	in_pRect->XminXmaxYminYmax[0]=rectFirstByte;
	if (in_pStream->read(in_pRect->XminXmaxYminYmax+1, 1, numberRectBytes-1)!=numberRectBytes-1)
		return false;

	return true;
}