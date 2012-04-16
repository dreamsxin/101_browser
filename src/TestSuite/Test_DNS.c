/*
* Copyright 2011-2012 Wolfgang Keller
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
#ifdef _WIN32
#include <Ws2tcpip.h> // for inet_pton - works only from Vista/Server 2008 on
#endif
#include "DNS/DNS.h"
#include <string.h>
#include <stdio.h>

void test_prepareQNAME()
{
	char buffer[128];
	const char emptyString[] = "";
	const char dotString[] = ".";
	const char reducedTwitter[] = "twitter.";
	const char twittercom[] = "twitter.com";
	/*
	* Q: Why this strange notation (also in the following results)?
	* A: If we use "\x07twitter\x03com" the Visual Studio compiler
	*    inserts a 0x3c character instead. I don't know whether
	*    this is a bug of the Visual Studio compiler or not.
	*/
	const char twittercomResult[] = "\x07twitter\x03" "com";

	const char wwwalumniovgude[] = "www.alumni.ovgu.de";
	const char wwwalumniovgudeResult[] = "\x03www\x06" "alumni\x04ovgu\x02" "de";

	const char overlong[] =
		"YYYYYYYYYYYYYYYY"
		"YYYYYYYYYYYYYYYY"
		"YYYYYYYYYYYYYYYY"
		"YYYYYYYYYYYYYYYY"
		".de";
	
	const char notOverlong[] =
		"YYYYYYYYYYYYYYYY"
		"YYYYYYYYYYYYYYYY"
		"YYYYYYYYYYYYYYYY"
		"YYYYYYYYYYYYYYY"
		".se";

	const char notOverlongResult[] = "\x3f" 
		"YYYYYYYYYYYYYYYY"
		"YYYYYYYYYYYYYYYY"
		"YYYYYYYYYYYYYYYY"
		"YYYYYYYYYYYYYYY"
		"\x02se";

	test(prepareOrCheckQNAME(buffer, emptyString, false));
	test(prepareOrCheckQNAME(buffer, dotString, false));
	test(prepareOrCheckQNAME(buffer, reducedTwitter, false));
	test(!prepareOrCheckQNAME(buffer, twittercom, false));
	test(!memcmp(buffer, twittercomResult, strlen(twittercomResult) + 1));
	test(!prepareOrCheckQNAME(buffer, wwwalumniovgude, false));
	test(!memcmp(buffer, wwwalumniovgudeResult, strlen(wwwalumniovgudeResult) + 1));

	test(prepareOrCheckQNAME(buffer, overlong, false));
	test(!prepareOrCheckQNAME(buffer, notOverlong, false));
	test(!memcmp(buffer, notOverlongResult, strlen(notOverlongResult) + 1));
}

void test_readDNS()
{
	char server[] = 
#if 1
		/*
		* Google DNS
		* has adresses
		* 8.8.8.8 and
		* 8.8.4.4
		* according to
		* http://code.google.com/intl/de/speed/public-dns/
		*/
		"8.8.8.8";
#endif
		/*
		* Google DNS' IPv6 adresses are
		* 2001:4860:4860::8888
		* 2001:4860:4860::8844
		* according to
		* https://groups.google.com/group/public-dns-announce/msg/938acdc14aeb96d0
		*/

#if 0
		"217.237.149.205";
#endif
#if 0
		"217.237.151.51";
#endif

	IN_ADDR serverAddr;

	/*
	* Reason for inet_pton: inet_addr has a problem (see 
	* http://www.manpagez.com/man/3/inet_addr/):
	* "BUGS
	*
	* The value INADDR_NONE (0xffffffff) is a valid broadcast address, but
	* inet_addr() cannot return that value without indicating failure.  The
	* newer inet_aton() function does not share this problem."
	* 
	* Unluckily inet_addr is not supported on Windows. So we use
	* inet_pton as given as answer on
	* http://stackoverflow.com/a/2422653
	*/
	if (inet_pton(AF_INET, server, 
		/*
		* http://msdn.microsoft.com/en-us/library/cc805844%28VS.85%29.aspx
		* "When the Family parameter is AF_INET, this buffer should be large enough
		* to hold an IN_ADDR structure."
		*/
		&serverAddr) != 1)
	{
		fprintf(stderr, "Error when using inet_pton.\n");
		exit(EXIT_FAILURE);
	}

	test(!readDNS(serverAddr, "twitter.com"));
	test(!readDNS(serverAddr, "blog.fefe.de"));
	test(!readDNS(serverAddr, "fsf.org"));
	test(!readDNS(serverAddr, "qwerzhgcysry.org"));
	test(!readDNS(serverAddr, "qwerzhgcysry.blu"));
	test(!readDNS(serverAddr, "se"));
}

void test_DNS()
{
	printf("Testing DNS - prepareQNAME\n");
	test_prepareQNAME();

	printf("Testing DNS - readDNS\n");
	test_readDNS();
}
