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

#ifndef _TestSuite_h
#define _TestSuite_h

#include "MiniStdlib/string_macros.h"

#ifdef __cplusplus
extern "C" {
#endif

#define test(_Expression) (void)( ((!!(_Expression)) && (testPassed(TO_STRING(#_Expression), TO_STRING(__FILE__), __LINE__), 1)) || (testFailed(TO_STRING(#_Expression), TO_STRING(__FILE__), __LINE__), 0) )

void testPassed(const char * _Message, const char *_File, unsigned int _Line);
void testFailed(const char * _Message, const char *_File, unsigned int _Line);

#ifdef __cplusplus
}
#endif

#endif
