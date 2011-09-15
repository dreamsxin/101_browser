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

#include "TestSuite/Tests.h"
#include "TestSuite/TestSuite.h"
#include "CRC/CRC.h"

void test_CRC()
{
	uint8_t buffer[] = {'I', 'E', 'N', 'D'};
	uint32_t currentCRC;

	// First a manual computing
	currentCRC = CRC_init(true);
	currentCRC = CRC_update_LSB_to_MSB(currentCRC, buffer[0]);
	currentCRC = CRC_update_LSB_to_MSB(currentCRC, buffer[1]);
	currentCRC = CRC_update_LSB_to_MSB(currentCRC, buffer[2]);
	currentCRC = CRC_update_LSB_to_MSB(currentCRC, buffer[3]);
	currentCRC = CRC_terminate(currentCRC, true);
	test(currentCRC == 0xAE426082);

	// Now an automatic computation
	currentCRC = CRC_compute_LSB_to_MSB(buffer, 4, true, true);
	test(currentCRC == 0xAE426082);
}
