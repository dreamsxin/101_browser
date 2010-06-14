#include "CryptoWin/CryptoWin.h"

namespace MTAx
{
	namespace Crypto
	{
		__declspec(dllexport) CryptoContext allocRandomNumberContext()
		{
			HCRYPTPROV hCryptProv = NULL;

			CryptAcquireContext(&hCryptProv, 
				NULL,
				MS_DEF_PROV,
				PROV_RSA_FULL, 
				CRYPT_VERIFYCONTEXT);

			return hCryptProv;
		}

		__declspec(dllexport) void freeRandomNumberContext(CryptoContext pContext)
		{
			CryptReleaseContext(pContext, 0);
		}

		__declspec(dllexport) void getRandomBytes(CryptoContext context, 
			size_t count, void* buffer)
		{
			CryptGenRandom(context, count, static_cast<BYTE*>(buffer));
		}
	}
}
