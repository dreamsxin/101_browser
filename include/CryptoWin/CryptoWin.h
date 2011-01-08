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
