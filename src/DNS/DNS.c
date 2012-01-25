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
#include "MiniStdlib/csetjmp.h"
#include <stdio.h> // for printf - only for testing

#pragma pack(push, 1)

#ifdef _MSC_VER
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

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#pragma pack(pop)

const uint16_t cDnsConstantID = 0x42; // TODO: Change
const uint8_t cDnsConstantRD  = 1;
#define DNS_CONSTANT_QDCOUNT (htons(1))

// 
/*
* QTYPE field
* See 3.2.2. TYPE values and 3.2.3. QTYPE values
*
* TYPE A
* Value: 1
* Meaning: a host address
*/
#define DNS_CONSTANT_QTYPE   (htons(1))

/*
* QCLASS field
* See 3.2.4. CLASS values and 3.2.5. QCLASS values
*
* CLASS IN
* Value: 1
* Meaning: the Internet
*/
#define DNS_CONSTANT_QCLASS  (htons(1))


int prepareOrCheckHeader(Header *in_out_pHeader, 
	bool in_checkAnswerForCorrectness)
{
	if (!in_checkAnswerForCorrectness)
		memset(in_out_pHeader, 0, sizeof(Header));

	if (!in_checkAnswerForCorrectness)
	{
		in_out_pHeader->ID = cDnsConstantID;
		in_out_pHeader->QR = 0;     // Query
		in_out_pHeader->OPCODE = 0; // 0 a standard query (QUERY)
		                            // 1  an inverse query (IQUERY)
		                            // 2  a server status request (STATUS)

		/*
		* RFC 1035 - 4.1.1. Header section format
		* "Recursion Desired - this bit may be set in a query and
		* is copied into the response.  If RD is set, it directs
		* the name server to pursue the query recursively.
		* Recursive query support is optional."
		*/
		in_out_pHeader->RD = cDnsConstantRD;

		/*
		* "QDCOUNT an unsigned 16 bit integer specifying the number of 
		* entries in the question section."
		*/
		in_out_pHeader->QDCOUNT = DNS_CONSTANT_QDCOUNT;
	}
	else
	{
		if (in_out_pHeader->ID != cDnsConstantID)
			return DnsReturnValueErrorInvalidResponse;
		if (in_out_pHeader->QR != 1) // Response
			return DnsReturnValueErrorInvalidResponse;
		if (in_out_pHeader->OPCODE != 0)  // 0 a standard query (QUERY)
		                                  // 1  an inverse query (IQUERY)
		                                  // 2  a server status request (STATUS)
			return DnsReturnValueErrorInvalidResponse;					                

		// AA may be 0 or 1
		// TC may be 0 or 1

		if (in_out_pHeader->RD != cDnsConstantRD)
			return DnsReturnValueErrorInvalidResponse;

		// RA may be 0 or 1

		if (((Header *) in_out_pHeader)->Z != 0)
			return DnsReturnValueErrorInvalidResponse;

		if (in_out_pHeader->QDCOUNT != DNS_CONSTANT_QDCOUNT)
			return DnsReturnValueErrorInvalidResponse;
	}

	return DnsReturnValueOK;
}

int prepareOrCheckQNAME(char *in_out_preQNAME, const char *in_pLabel, 
	bool in_checkAnswerForCorrectness)
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
	
	const char *pCurrentCharacterInSource = in_pLabel;
	char *pCurrentCharacterInDestination = in_out_preQNAME + 1;
	char *pCurrentLength = in_out_preQNAME;
	// bytesCount == 0 <=> state == 0
	uint8_t bytesCount = 0;

	assert(in_out_preQNAME != NULL);
	assert(in_pLabel != NULL);

	while (*pCurrentCharacterInSource)
	{
		if ('.' == *pCurrentCharacterInSource)
		{
			if (0 == bytesCount)
			{
				// Failure
				if (!in_checkAnswerForCorrectness)
					return DnsReturnValueErrorInvalidInput;
				else
					return DnsReturnValueErrorInvalidResponse;
			}
			else
			{
				if (!in_checkAnswerForCorrectness)
					*pCurrentLength = (char) bytesCount;
				else
				{
					if (*(uint8_t*) pCurrentLength != bytesCount)
						return DnsReturnValueErrorInvalidResponse;
				}

				bytesCount = 0;
				pCurrentLength = pCurrentCharacterInDestination;
			}
		}
		else
		{
			if (63 == bytesCount)
			{
				// Failure
				if (!in_checkAnswerForCorrectness)
					return DnsReturnValueErrorInvalidInput;
				else
					return DnsReturnValueErrorInvalidResponse;
			}

			if (!in_checkAnswerForCorrectness)
				*pCurrentCharacterInDestination = *pCurrentCharacterInSource;
			else
			{
				if (*pCurrentCharacterInDestination != *pCurrentCharacterInSource)
					return DnsReturnValueErrorInvalidResponse;
			}
			bytesCount++;
		}

		pCurrentCharacterInSource++;
		pCurrentCharacterInDestination++;
	}

	if (0 == bytesCount)
	{
		// Failure
		if (!in_checkAnswerForCorrectness)
			return DnsReturnValueErrorInvalidInput;
		else
			return DnsReturnValueErrorInvalidResponse;
	}
	else
	{
		assert(0x0 == *pCurrentCharacterInSource);
		if (!in_checkAnswerForCorrectness)
		{
			*pCurrentCharacterInDestination = *pCurrentCharacterInSource;
			*pCurrentLength = (char) bytesCount;
		}
		else
		{
			if (*pCurrentCharacterInDestination != *pCurrentCharacterInSource)
				return DnsReturnValueErrorInvalidResponse;
			if (*(uint8_t*) pCurrentLength != bytesCount)
				return DnsReturnValueErrorInvalidResponse;
		}

		// Success
		return DnsReturnValueOK;
	}
}

int readName(uint8_t **in_out_ppName, size_t *in_out_pBufferSize)
{
	if (0 == in_out_pBufferSize)
		return -2;
	
	if (0xC0 == (0xC0 & (*in_out_ppName)[0]))
	{
		if (*in_out_pBufferSize < 2)
			return -2;
		
		// TODO: Test for correct value (i. e. sensible offset)
		(*in_out_ppName) += 2;
		(*in_out_pBufferSize) -= 2;
		return 0;
	}
	// TODO: More general string parsing (unluckily I have no test cases :-( )
	else if (0x0 == (*in_out_ppName)[0])
	{
		(*in_out_ppName)++;
		(*in_out_pBufferSize)--;
		return 0;
	}
	else
		return -3; // Not implemented
}

int handleRessourceRecords(size_t in_rrCount, 
	uint8_t *in_pointerTowardsBeginOfResponse, size_t in_remainingSize)
{
	unsigned int currentRRIndex;
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
		pRRMiddlePart->CLASS = ntohs(pRRMiddlePart->CLASS);
		pRRMiddlePart->TTL = ntohl(pRRMiddlePart->TTL);
		pRRMiddlePart->RDLENGTH = ntohs(pRRMiddlePart->RDLENGTH);

		printf("\tTYPE = %u\n"
			"\tCLASS = %u\n"
			"\tTTL = %u\n"
			"\tRDLENGTH = %u\n",
			pRRMiddlePart->TYPE, pRRMiddlePart->CLASS, 
			pRRMiddlePart->TTL, pRRMiddlePart->RDLENGTH);

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


int prepareOrCheckPackage(const char *in_cDomain, char *in_pBuffer, int *in_out_pBufferSize, 
	bool in_checkAnswerForCorrectness)
{
	size_t domainLen = strlen(in_cDomain);
	int bufferSize;
	int result;
	
	if (domainLen > 512 - sizeof(Header) - 1 - domainLen - 2 - 2)
		return DnsReturnValueErrorInvalidInput;

	bufferSize = sizeof(Header)
		+ 1  // first length byte
		+ domainLen
		+ 1  // zero byte terminating the string
		+ 2  // QTYPE
		+ 2; // QNAME

	assert(bufferSize <= 512);

	if (!in_checkAnswerForCorrectness)
	{
		*in_out_pBufferSize = bufferSize;
	}
	else
	{
		if (bufferSize > *in_out_pBufferSize)
			return DnsReturnValueErrorInvalidResponse;
	}

	result = prepareOrCheckHeader((Header *) in_pBuffer, in_checkAnswerForCorrectness);

	if (!in_checkAnswerForCorrectness)
		assert(0 == result);

	if (result)
		return result;

	if ((result = prepareOrCheckQNAME(in_pBuffer + sizeof(Header), in_cDomain, 
		in_checkAnswerForCorrectness)) != 0)
		return result;

	if (!in_checkAnswerForCorrectness)
	{
		*((uint16_t*) (in_pBuffer + sizeof(Header) + 1 + domainLen + 1))     = DNS_CONSTANT_QTYPE;
		*((uint16_t*) (in_pBuffer + sizeof(Header) + 1 + domainLen + 1 + 2)) = DNS_CONSTANT_QCLASS;
	}
	else
	{
		if (*((uint16_t*) (in_pBuffer + sizeof(Header) + 1 + domainLen + 1))     != DNS_CONSTANT_QTYPE)
			return DnsReturnValueErrorInvalidResponse;
		if (*((uint16_t*) (in_pBuffer + sizeof(Header) + 1 + domainLen + 1 + 2)) != DNS_CONSTANT_QCLASS)
			return DnsReturnValueErrorInvalidResponse;
	}

	return DnsReturnValueOK;
}


int readDNS(const char *in_cDnsServer, const char *in_cDomain)
{
	socket_t udpSocket = 0;
	sockaddr_in_t udpAddr;
	sockaddr_in_t remoteAddr;
	socklen_t remoteAddrLen = sizeof(remoteAddr);
	int result;
	char buffer[512];
	int buffer0Size;
	int buffer1Size;
	
	uint8_t *pointerTowardsBeginOfResponse = NULL;
	size_t remainingSize = 0;
	jmp_buf jmpBuf;

	if ((result = setjmp(jmpBuf)) != 0)
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
		return DnsReturnValueErrorNetworkError;

	udpAddr.sin_family = AF_INET;
	udpAddr.sin_port = htons(53);
	
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
	if (inet_pton(AF_INET, in_cDnsServer, 
		/*
		* http://msdn.microsoft.com/en-us/library/cc805844%28VS.85%29.aspx
		* "When the Family parameter is AF_INET, this buffer should be large enough
		* to hold an IN_ADDR structure."
		*/
		&udpAddr.sin_addr) != 1)
		longjmp(jmpBuf, -1);

	prepareOrCheckPackage(in_cDomain, buffer, &buffer0Size, false);

	result = sendto(udpSocket, buffer, buffer0Size, 0, (sockaddr_t *) &udpAddr, sizeof(udpAddr));

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
		longjmp(jmpBuf, DnsReturnValueErrorNetworkError);

	assert(buffer0Size == result);

	buffer1Size = recvfrom(udpSocket, buffer, sizeof(buffer), 0, (sockaddr_t *) &remoteAddr, &remoteAddrLen);
	
	if (SOCKET_ERROR == buffer1Size)
		longjmp(jmpBuf, DnsReturnValueErrorNetworkError);
	
	if ((result = prepareOrCheckPackage(in_cDomain, buffer, &buffer1Size, true)) != 0)
		longjmp(jmpBuf, result);

	// Gets checked in prepareOrCheckPackage
	assert(buffer1Size >= buffer0Size);
	// Check AA
	printf("AA = %u\n", ((Header *) buffer)->AA);
	// Check TC
	printf("TC = %u\n", ((Header *) buffer)->TC);

	// Check RA
	printf("RA = %u\n", ((Header *) buffer)->RA);

	// Check RCODE
	printf("RCODE = %u\n", ((Header *) buffer)->RCODE);

	((Header *) buffer)->ANCOUNT = ntohs(((Header *) buffer)->ANCOUNT);
	((Header *) buffer)->NSCOUNT = ntohs(((Header *) buffer)->NSCOUNT);
	((Header *) buffer)->ARCOUNT = ntohs(((Header *) buffer)->ARCOUNT);

	// Check ANCOUNT
	printf("ANCOUNT = %u\n", ((Header *) buffer)->ANCOUNT);
	// Check NSCOUNT
	printf("NSCOUNT = %u\n", ((Header *) buffer)->NSCOUNT);
	// Check ARCOUNT
	printf("ARCOUNT = %u\n", ((Header *) buffer)->ARCOUNT);

	pointerTowardsBeginOfResponse = (uint8_t *) buffer + buffer0Size;
	
	// Is >= 0 because of CND:DNS_c_275
	remainingSize = buffer1Size - buffer0Size;

	if (0 == ((Header *) buffer)->RCODE)
	{
		if (0 != ((Header *) buffer)->ARCOUNT)
			longjmp(jmpBuf, DnsReturnValueNotImplemented);

		// Either ANCOUNT or NSCOUNT must be != 0 (but exactly one of them)
		
		if (0 != ((Header *) buffer)->ANCOUNT)
		{
			if (0 != ((Header *) buffer)->NSCOUNT)
				longjmp(jmpBuf, DnsReturnValueNotImplemented);

			assert(0 != ((Header *) buffer)->ANCOUNT);
			assert(0 == ((Header *) buffer)->NSCOUNT);
			assert(0 == ((Header *) buffer)->ARCOUNT);

			if ((result = handleRessourceRecords(((Header *) buffer)->ANCOUNT, 
				pointerTowardsBeginOfResponse, remainingSize)) != 0)
				longjmp(jmpBuf, result);
		}
		else
		{
			if (0 == ((Header *) buffer)->NSCOUNT)
				longjmp(jmpBuf, DnsReturnValueNotImplemented);

			assert(0 == ((Header *) buffer)->ANCOUNT);
			assert(0 != ((Header *) buffer)->NSCOUNT);
			assert(0 == ((Header *) buffer)->ARCOUNT);

			if ((result = handleRessourceRecords(((Header *) buffer)->NSCOUNT, 
				pointerTowardsBeginOfResponse, remainingSize)) != 0)
				longjmp(jmpBuf, result);
		}
	}
	else if (3 == ((Header *) buffer)->RCODE)
	{
		if (0 != ((Header *) buffer)->ANCOUNT)
			longjmp(jmpBuf, DnsReturnValueNotImplemented);

		if (0 == ((Header *) buffer)->NSCOUNT)
			longjmp(jmpBuf, DnsReturnValueNotImplemented);

		if (0 != ((Header *) buffer)->ARCOUNT)
			longjmp(jmpBuf, DnsReturnValueNotImplemented);

		assert(0 == ((Header *) buffer)->ANCOUNT);
		assert(((Header *) buffer)->NSCOUNT > 0);
		assert(0 == ((Header *) buffer)->ARCOUNT);
		
		if ((result = handleRessourceRecords(((Header *) buffer)->NSCOUNT, 
			pointerTowardsBeginOfResponse, remainingSize)) != 0)
			longjmp(jmpBuf, result);
	}
	else
		longjmp(jmpBuf, DnsReturnValueNotImplemented);

	closesocket(udpSocket);
	return DnsReturnValueOK;
}
