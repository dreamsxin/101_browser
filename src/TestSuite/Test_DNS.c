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

#include "TestSuite/Tests.h"
#include "TestSuite/TestSuite.h"
#include "DNS/DNS.h"
#include <string.h>
#include <stdio.h>

void test_prepareQNAME()
{
	char emptyString[] = "X";
	char dotString[] = "X.";
	char reducedTwitter[] = "Xtwitter.";
	char twittercom[] = "Xtwitter.com";
	/*
	* Q: Why this strange notation (also in the following results)?
	* A: If we use "\x07twitter\x03com" the Visual Studio compiler
	*    inserts a 0x3c character instead. I don't know whether
	*    this is a bug of the Visual Studio compiler or not.
	*/
	char twittercomResult[] = "\x07twitter\x03" "com";

	char wwwalumniovgude[] = "Xwww.alumni.ovgu.de";
	char wwwalumniovgudeResult[] = "\x03www\x06" "alumni\x04ovgu\x02" "de";

	char overlong[] = "X" 
		"YYYYYYYYYYYYYYYY"
		"YYYYYYYYYYYYYYYY"
		"YYYYYYYYYYYYYYYY"
		"YYYYYYYYYYYYYYYY"
		"YYYYYYYYYYYYYYYY"
		"YYYYYYYYYYYYYYYY"
		"YYYYYYYYYYYYYYYY"
		"YYYYYYYYYYYYYYYY"
		"YYYYYYYYYYYYYYYY"
		"YYYYYYYYYYYYYYYY"
		"YYYYYYYYYYYYYYYY"
		"YYYYYYYYYYYYYYYY"
		"YYYYYYYYYYYYYYYY"
		"YYYYYYYYYYYYYYYY"
		"YYYYYYYYYYYYYYYY"
		"YYYYYYYYYYYYYYYY"
		".de";
	
	char notOverlong[] = "X" 
		"YYYYYYYYYYYYYYYY"
		"YYYYYYYYYYYYYYYY"
		"YYYYYYYYYYYYYYYY"
		"YYYYYYYYYYYYYYYY"
		"YYYYYYYYYYYYYYYY"
		"YYYYYYYYYYYYYYYY"
		"YYYYYYYYYYYYYYYY"
		"YYYYYYYYYYYYYYYY"
		"YYYYYYYYYYYYYYYY"
		"YYYYYYYYYYYYYYYY"
		"YYYYYYYYYYYYYYYY"
		"YYYYYYYYYYYYYYYY"
		"YYYYYYYYYYYYYYYY"
		"YYYYYYYYYYYYYYYY"
		"YYYYYYYYYYYYYYYY"
		"YYYYYYYYYYYYYYY"
		".se";

	char notOverlongResult[] = "\xff" 
		"YYYYYYYYYYYYYYYY"
		"YYYYYYYYYYYYYYYY"
		"YYYYYYYYYYYYYYYY"
		"YYYYYYYYYYYYYYYY"
		"YYYYYYYYYYYYYYYY"
		"YYYYYYYYYYYYYYYY"
		"YYYYYYYYYYYYYYYY"
		"YYYYYYYYYYYYYYYY"
		"YYYYYYYYYYYYYYYY"
		"YYYYYYYYYYYYYYYY"
		"YYYYYYYYYYYYYYYY"
		"YYYYYYYYYYYYYYYY"
		"YYYYYYYYYYYYYYYY"
		"YYYYYYYYYYYYYYYY"
		"YYYYYYYYYYYYYYYY"
		"YYYYYYYYYYYYYYY"
		"\x2se";

	test(prepareQNAME(emptyString));
	test(prepareQNAME(dotString));
	test(prepareQNAME(reducedTwitter));
	test(!prepareQNAME(twittercom));
	test(!memcmp(twittercom, twittercomResult, strlen(twittercomResult) + 1));
	test(!prepareQNAME(wwwalumniovgude));
	test(!memcmp(wwwalumniovgude, wwwalumniovgudeResult, strlen(wwwalumniovgudeResult) + 1));

	test(prepareQNAME(overlong));
	test(!prepareQNAME(notOverlong));
	test(!memcmp(notOverlong, notOverlongResult, strlen(notOverlongResult) + 1));
}

void test_readDNS()
{
	test(!readDNS("8.8.8.8", "twitter.com"));
}

void test_DNS()
{
	printf("Testing DNS - prepareQNAME\n");
	test_prepareQNAME();

	printf("Testing DNS - readDNS\n");
	test_readDNS();
}