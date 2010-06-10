#ifndef _Endianess_h
#define _Endianess_h

namespace MTAx
{
	namespace Endianess
	{
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
	};
}

#endif
