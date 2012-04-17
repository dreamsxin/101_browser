/*
* Copyright 2012 Wolfgang Keller
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

#ifndef _Unicode_Parser_h
#define _Unicode_Parser_h

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
	/*
	* ParseBlocker_Neither is returned when parsing is terminated successfully 
	* and there is no need to proceed further.
	*/
	ParseBlocker_Neither,
	/*
	* ParseBlocker_Reader and ParseBlocker_Writer are returned when there 
	* is a problem (either intervention required or error) in the reader 
	* or writer, respectively.
	*/
	ParseBlocker_Reader,
	ParseBlocker_Writer
} ParseBlocker;

#ifdef __cplusplus
}
#endif

#endif
