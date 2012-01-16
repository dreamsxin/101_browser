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

#include "DNS/DNS.h"
#include "Network/Network.h"
#ifdef _WIN32
#include <Ws2tcpip.h> // for inet_pton - works only from Vista/Server 2008 on
#endif
#include <assert.h>
#include "MiniStdlib/cstdint.h"
#include "MiniStdlib/cstring.h"
#include "MiniStdlib/cstdbool.h"
#include "MiniStdlib/csetjmp.h"
#include <stdio.h> // for printf - only for testing

#pragma pack(push, 1)

#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable : 4214)
#endif

typedef struct
{
	uint16_t ID;
	uint8_t RD : 1;
	uint8_t TC : 1;
	uint8_t AA : 1;
	uint8_t OPCODE : 4;
	uint8_t QR : 1;
	
	uint8_t RCODE : 4;
	uint8_t Z : 3;
	uint8_t RA : 1;

	uint16_t QDCOUNT;
	uint16_t ANCOUNT;
	uint16_t NSCOUNT;
	uint16_t ARCOUNT;
} Header;

typedef struct
{
	uint16_t TYPE;
	uint16_t CLASS;
	uint32_t TTL;
	uint16_t RDLENGTH;
} RessourceRecordMiddlePart;

#ifdef _WIN32
#pragma warning(pop)
#endif

#pragma pack(pop)

void fillHeader(Header *out_pHeader)
{
	memset(out_pHeader, 0, sizeof(Header));

	out_pHeader->ID = 0;     // TODO: Change
	out_pHeader->QR = 0;     // Query
	out_pHeader->OPCODE = 0; // 0 a standard query (QUERY)
	                         // 1  an inverse query (IQUERY)
	                         // 2  a server status request (STATUS)
	/*
	* RFC 1035 - 4.1.1. Header section format
	* "Recursion Desired - this bit may be set in a query and
	* is copied into the response.  If RD is set, it directs
	* the name server to pursue the query recursively.
	* Recursive query support is optional."
	*/
	out_pHeader->RD = 1;

	/*
	* "QDCOUNT an unsigned 16 bit integer specifying the number of 
	* entries in the question section."
	*/
	out_pHeader->QDCOUNT = ntohs(1);
}

int prepareQNAME(char *in_out_preQNAME)
{
	/*
	* Automaton for parsing QNAMEs (not including that each label must not
	* be larger than 256 octets).
	*
	*
	*  != '.', 0
	*    ┌───┐
	*    │   V
	*   ┌─────┐    '.'     ┌─────┐
	*   │     │───────────>│     │
	*   │  1  │	           │  0  │<
	*   │     │<───────────│     │
	*   └─────┘	 != '.', 0 └─────┘
	*      │                  │
	*      │ 0                │ '.'
	*      │                  │
	*      V                  V
	*   ╔═════╗          ╔═════════╗
	*   ║ OK  ║          ║ FAILURE ║
	*   ╚═════╝          ╚═════════╝
	*/
	
	char *pCurrentLength = in_out_preQNAME;
	char *pCurrentCharacterInLabel = in_out_preQNAME + 1;
	// bytesCount == 0 <=> state == 0
	uint8_t bytesCount = 0;

	while (*pCurrentCharacterInLabel)
	{
		if ('.' == *pCurrentCharacterInLabel)
		{
			if (0 == bytesCount)
				// Failure
				return 1;
			else
			{
				*pCurrentLength = (char) bytesCount;
				bytesCount = 0;
				pCurrentLength = pCurrentCharacterInLabel;
			}
		}
		else
		{
			if (63 == bytesCount)
				// Failure
				return 1;
			bytesCount++;
		}

		pCurrentCharacterInLabel++;
	}

	if (0 == bytesCount)
		// Failure
		return 1;
	else
	{
		*pCurrentLength = (char) bytesCount;
		// Success
		return 0;
	}
}

int readName(uint8_t **in_out_ppName, size_t *in_out_pBufferSize)
{
	if (*in_out_pBufferSize < 2)
		return -2;
	
	if (0xC0 == (*in_out_ppName)[0] && 0x0C == (*in_out_ppName)[1])
	{
		(*in_out_ppName) += 2;
		(*in_out_pBufferSize) -= 2;
		return 0;
	}
	else
		return -3; // Not implemented
}

int handleRessourceRecords(size_t in_rrCount, 
	uint8_t *in_pointerTowardsBeginOfResponse, size_t in_remainingSize)
{
	int currentRRIndex;
	int result;

	assert(in_rrCount > 0);

	for (currentRRIndex = 0; currentRRIndex < in_rrCount; currentRRIndex++)
	{
		RessourceRecordMiddlePart *pRRMiddlePart;

		result = readName(&in_pointerTowardsBeginOfResponse, &in_remainingSize);

		if (result)
			return result;

		if (in_remainingSize < sizeof(RessourceRecordMiddlePart))
			return -2;

		pRRMiddlePart = (RessourceRecordMiddlePart*) in_pointerTowardsBeginOfResponse;
		pRRMiddlePart->TYPE = ntohs(pRRMiddlePart->TYPE);
		pRRMiddlePart->CLASS = ntohs(pRRMiddlePart->TYPE);
		pRRMiddlePart->TTL = ntohl(pRRMiddlePart->TTL);
		pRRMiddlePart->RDLENGTH = ntohs(pRRMiddlePart->RDLENGTH);

		in_pointerTowardsBeginOfResponse += sizeof(RessourceRecordMiddlePart);
		in_remainingSize -= sizeof(RessourceRecordMiddlePart);

		if (in_remainingSize < pRRMiddlePart->RDLENGTH)
			return -2;

		in_pointerTowardsBeginOfResponse += pRRMiddlePart->RDLENGTH;
		in_remainingSize -= pRRMiddlePart->RDLENGTH;
	}

	if (in_remainingSize != 0)
		return -2;

	return 0;
}

int readDNS(const char *in_cDnsServer, const char *in_cDomain)
{
	socket_t udpSocket = 0;
	sockaddr_in_t udpAddr;
	sockaddr_in_t remoteAddr;
	socklen_t remoteAddrLen = sizeof(remoteAddr);
	int result;
	char buffer0[512];
	int buffer0Size;
	char buffer1[512];
	int buffer1Size;
	size_t domainLen = strlen(in_cDomain);
	uint8_t *pointerTowardsBeginOfResponse = NULL;
	size_t remainingSize = 0;
	jmp_buf jmpBuf;

	if (result = setjmp(jmpBuf))
	{
		if (udpSocket)
			closesocket(udpSocket);

		return result;
	}
	
	/*
	* Comments taken from
	* http://msdn.microsoft.com/en-us/library/windows/desktop/ms740506(v=vs.85).aspx
	*/
	udpSocket = socket(
		/*
		* The Internet Protocol version 4 (IPv4) address family.
		*/
		AF_INET,
		/*
		* A socket type that supports datagrams, which are connectionless, 
		* unreliable buffers of a fixed (typically small) maximum length. This 
		* socket type uses the User Datagram Protocol (UDP) for the Internet 
		* address family (AF_INET or AF_INET6).
		*/
		SOCK_DGRAM,
		/*
		* The User Datagram Protocol (UDP). This is a possible value when 
		* the af parameter is AF_INET or AF_INET6 and the type parameter 
		* is SOCK_DGRAM.
		*/
		IPPROTO_UDP);

	if (INVALID_SOCKET == udpSocket)
		return -1;

	udpAddr.sin_family = AF_INET;
	udpAddr.sin_port = htons(53);
	// TODO: Use inet_pton inet_addr (Reason: http://stackoverflow.com/a/2422653)
	
	if (inet_pton(AF_INET, in_cDnsServer, 
		/*
		* http://msdn.microsoft.com/en-us/library/cc805844%28VS.85%29.aspx
		* "When the Family parameter is AF_INET, this buffer should be large enough
		* to hold an IN_ADDR structure."
		*/
		&udpAddr.sin_addr) != 1)
		longjmp(jmpBuf, -1);

	buffer0Size = sizeof(Header)
		+ 1  // length byte
		+ domainLen
		+ 1  // zero byte terminating the string
		+ 2  // QTYPE
		+ 2; // QNAME

	if (buffer0Size > 512)
		longjmp(jmpBuf, -1);

	fillHeader((Header *) buffer0);

	memcpy(buffer0 + sizeof(Header) + 1, in_cDomain, domainLen + 1);

	if (prepareQNAME(buffer0 + sizeof(Header)))
		longjmp(jmpBuf, -1);

	// QTYPE field
	// See 3.2.2. TYPE values and 3.2.3. QTYPE values

	/*
	* TYPE A
	* Value: 1
	* Meaning: a host address
	*/
	*((uint16_t*) (buffer0 + sizeof(Header) + 1 + domainLen + 1)) = ntohs(1);

	// QCLASS field
	// See 3.2.4. CLASS values and 3.2.5. QCLASS values

	/*
	* CLASS IN
	* Value: 1
	* Meaning: the Internet
	*/
	*((uint16_t*) (buffer0 + sizeof(Header) + 1 + domainLen + 1 + 2)) = ntohs(1);
	
	result = sendto(udpSocket, buffer0, buffer0Size, 0, (sockaddr_t *) &udpAddr, sizeof(udpAddr));

	/*
	* To quote
	* http://msdn.microsoft.com/en-us/library/windows/desktop/ms740148(v=vs.85).aspx
	*
	* "If no error occurs, sendto returns the total number of bytes sent, 
	* which can be less than the number indicated by len. Otherwise, a value 
	* of SOCKET_ERROR is returned, and a specific error code can be retrieved 
	* by calling WSAGetLastError."
	*/
	if (SOCKET_ERROR == result)
		longjmp(jmpBuf, -1);

	assert(buffer0Size == result);

	buffer1Size = recvfrom(udpSocket, buffer1, sizeof(buffer1), 0, (sockaddr_t *) &remoteAddr, &remoteAddrLen);
	
	if (SOCKET_ERROR == buffer1Size)
		longjmp(jmpBuf, -1);
	
	// CND:DNS_c_275
	if (buffer1Size < buffer0Size)
		longjmp(jmpBuf, -2);

	if (((Header *) buffer1)->ID != ((Header *) buffer0)->ID)
		longjmp(jmpBuf, -2);

	if (((Header *) buffer1)->QR != 1)
		longjmp(jmpBuf, -2);

	if (((Header *) buffer1)->OPCODE != ((Header *) buffer0)->OPCODE)
		longjmp(jmpBuf, -2);

	// Check AA
	printf("AA = %u\n", ((Header *) buffer1)->AA);
	// Check TC
	printf("TC = %u\n", ((Header *) buffer1)->TC);

	if (((Header *) buffer1)->RD != ((Header *) buffer0)->RD)
		longjmp(jmpBuf, -2);

	// Check RA
	printf("RA = %u\n", ((Header *) buffer1)->RA);

	if (((Header *) buffer1)->Z != 0)
		longjmp(jmpBuf, -2);

	// Check RCODE
	printf("RCODE = %u\n", ((Header *) buffer1)->RCODE);

	if (((Header *) buffer1)->QDCOUNT != ((Header *) buffer0)->QDCOUNT)
		longjmp(jmpBuf, -2);

	((Header *) buffer1)->ANCOUNT = ntohs(((Header *) buffer1)->ANCOUNT);
	((Header *) buffer1)->NSCOUNT = ntohs(((Header *) buffer1)->NSCOUNT);
	((Header *) buffer1)->ARCOUNT = ntohs(((Header *) buffer1)->ARCOUNT);

	// Check ANCOUNT
	printf("ANCOUNT = %u\n", ((Header *) buffer1)->ANCOUNT);
	// Check NSCOUNT
	printf("NSCOUNT = %u\n", ((Header *) buffer1)->NSCOUNT);
	// Check ARCOUNT
	printf("ARCOUNT = %u\n", ((Header *) buffer1)->ARCOUNT);

	if (memcmp(buffer1+sizeof(Header), buffer0+sizeof(Header), buffer0Size-sizeof(Header)))
		longjmp(jmpBuf, -2);

	pointerTowardsBeginOfResponse = (uint8_t *) buffer1 + buffer0Size;
	
	// Is >= 0 because of CND:DNS_c_275
	remainingSize = buffer1Size - buffer0Size;

	if (0 == ((Header *) buffer1)->RCODE)
	{
		if (0 != ((Header *) buffer1)->ARCOUNT)
			longjmp(jmpBuf, -3);

		// Either ANCOUNT or NSCOUNT must be != 0 (but exactly one of them)
		
		if (0 != ((Header *) buffer1)->ANCOUNT)
		{
			if (0 != ((Header *) buffer1)->NSCOUNT)
				longjmp(jmpBuf, -3);

			assert(0 != ((Header *) buffer1)->ANCOUNT);
			assert(0 == ((Header *) buffer1)->NSCOUNT);
			assert(0 == ((Header *) buffer1)->ARCOUNT);

			handleRessourceRecords(((Header *) buffer1)->ANCOUNT, 
				pointerTowardsBeginOfResponse, remainingSize);
		}
		else
		{
			if (0 == ((Header *) buffer1)->NSCOUNT)
				longjmp(jmpBuf, -3);

			assert(0 == ((Header *) buffer1)->ANCOUNT);
			assert(0 != ((Header *) buffer1)->NSCOUNT);
			assert(0 == ((Header *) buffer1)->ARCOUNT);

			handleRessourceRecords(((Header *) buffer1)->NSCOUNT, 
				pointerTowardsBeginOfResponse, remainingSize);
		}
	}
	else if (3 == ((Header *) buffer1)->RCODE)
	{
		if (0 != ((Header *) buffer1)->ANCOUNT)
			longjmp(jmpBuf, -3);

		if (0 == ((Header *) buffer1)->NSCOUNT)
			longjmp(jmpBuf, -3);

		if (0 != ((Header *) buffer1)->ARCOUNT)
			longjmp(jmpBuf, -3);

		assert(0 == ((Header *) buffer1)->ANCOUNT);
		assert(((Header *) buffer1)->NSCOUNT > 0);
		assert(0 == ((Header *) buffer1)->ARCOUNT);
		
		handleRessourceRecords(((Header *) buffer1)->NSCOUNT, 
			pointerTowardsBeginOfResponse, remainingSize);
	}
	else
		longjmp(jmpBuf, -3);

	closesocket(udpSocket);
	return 0;
}
