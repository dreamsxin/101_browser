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

#ifndef _FontServerUtil_h
#define _FontServerUtil_h

// TODO: Move into BasicDataStructures

/*!
 * Computes the floor (rounding down if necessary) of the binary logarithm
 * 
 * If 0 is given we return 0xFF 
 */
inline unsigned short floorLog2(unsigned short value)
{
	for (signed char i=15; i>=0; i--)
	{
		if (value & (1u<<i))
		{
			return i;
		}
	}

	return 0xFF;
}

#endif
