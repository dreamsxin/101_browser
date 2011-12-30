/*
* Copyright 2011 Wolfgang Keller
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

#ifndef _MTAx_DNS_DNS_h
#define _MTAx_DNS_DNS_h

#include "MiniStdlib/declspec.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
* Parameters:
* in_out_preQNAME - an arbitrary byte followed by a string
* 
* Return value: 0 - success
*               1 - failure
* 
* This function replaces all dots ('.') (and the first byte) by the length
* of the label following. If any label has a length of >= 64 or is empty, 1 is 
* returned and the content of in_out_preQNAME is not defined.
* 
* You probably don't want to use this function directly. It is only marked as 
* DLLEXPORT to enable testing.
*/
DLLEXPORT int prepareQNAME(char *in_out_preQNAME);

/*
* Assumption: startupNetwork() has successfully been called before.
*/
DLLEXPORT int readDNS(const char *in_cDnsServer, const char *in_cDomain);

#ifdef __cplusplus
}
#endif

#endif
