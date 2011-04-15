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

#ifndef _Tests_h
#define _Tests_h

#ifdef __cplusplus
extern "C" {
#endif
void testAlgorithm();
void testGIF();
#ifdef __cplusplus
}
#endif

void testInterval();
void testBasicDataStructuresTree();
void testTriangleStripBorderIterator();
void testOuterBorderIterator();
void testIteratorInstances();

#ifdef __cplusplus
extern "C" {
#endif

void testBigNumber();
void testASCIIStringUnicodeIterator();
void testHTML5();
void test_MiniStdlib();
void test_PNG();
void test_RFC1950();
void test_RFC1951();
void test_Coroutine();
void test_IO();

#ifdef __cplusplus
}
#endif

void testUnicode();

#endif
