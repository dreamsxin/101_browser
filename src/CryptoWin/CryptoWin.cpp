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

#include "CryptoWin/CryptoWin.h"

namespace MTAx
{
	namespace Crypto
	{
		CryptoContext allocRandomNumberContext()
		{
			HCRYPTPROV hCryptProv = NULL;

			CryptAcquireContext(&hCryptProv, 
				NULL,
				MS_DEF_PROV,
				PROV_RSA_FULL, 
				CRYPT_VERIFYCONTEXT);

			return hCryptProv;
		}

		void freeRandomNumberContext(CryptoContext pContext)
		{
			CryptReleaseContext(pContext, 0);
		}

		void getRandomBytes(CryptoContext context, 
			size_t count, void* buffer)
		{
			CryptGenRandom(context, count, static_cast<BYTE*>(buffer));
		}
	}
}
