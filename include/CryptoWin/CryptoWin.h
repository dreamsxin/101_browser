#ifndef _CryptoWin_h
#define _CryptoWin_h

#include <windows.h>

/*!
 * if you ask where MTAx comes from: since 101 is not a
 * valid namespace name, I used MTAx, which is 101 encoded
 * in Base64
 */
namespace MTAx
{
	namespace Crypto
	{
		typedef HCRYPTPROV CryptoContext;

		__declspec(dllexport) CryptoContext allocRandomNumberContext();
		__declspec(dllexport) void freeRandomNumberContext(CryptoContext pContext);
		__declspec(dllexport) void getRandomBytes(CryptoContext context, size_t count, void* buffer);
	}
}

#endif
