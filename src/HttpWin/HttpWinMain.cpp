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

#include "Network/Network.h"

#include <cassert>
#include <cstdio>
#include <cstring>
#include <winsock2.h>
#include <ws2tcpip.h>

char GETstring[] = "GET";

void printUsageAndExit(char* argv0)
{
	printf(
		"Usage: %s COMMAND protocol://url\n"
		"Where protocol is http (no other protocol supported at the moment)\n", argv0);
	exit(0);
}

char port80String[] = "80";

/*!
 * Parses the url
 *
 * Note that if in_pcUrl[*in_pAfterDomainPos] we have to send a slash
 * in the http request.
 * 
 * Return value:
 * true on success
 * false on failure
 */
bool parseURL(const char* in_pcUrl, size_t* in_pDomainPos, 
			  size_t* in_pAfterDomainPos)
{
	size_t lPosition = 0;
	if (in_pcUrl[lPosition] != 'h')
		return false;

	lPosition++;
	if (in_pcUrl[lPosition] != 't')
		return false;

	lPosition++;
	if (in_pcUrl[lPosition] != 't')
		return false;

	lPosition++;
	if (in_pcUrl[lPosition] != 'p')
		return false;

	lPosition++;
	if (in_pcUrl[lPosition] != ':')
		return false;

	lPosition++;
	if (in_pcUrl[lPosition] != '/')
		return false;

	lPosition++;
	if (in_pcUrl[lPosition] != '/')
		return false;

	size_t lDomainPos = lPosition+1;

	do
	{
		lPosition++;
	} while (in_pcUrl[lPosition] != 0x0 && in_pcUrl[lPosition] != '/');

	*in_pDomainPos = lDomainPos;
	*in_pAfterDomainPos = lPosition;
	return true;
}

int main(int argc, char** argv)
{
	if (argc <= 2)
	{
		printUsageAndExit(argv[0]);
	}

	char *url = argv[argc-1];
	char *command;

	if (argc > 2)
	{
		command = argv[argc-2];
	}
	else
	{
		command = GETstring;
	}

	size_t domainPos;
	size_t afterDomainPos;

	if (!parseURL(url, &domainPos, &afterDomainPos))
	{
		printUsageAndExit(argv[0]);
	}

	if (url[afterDomainPos] != 0x0)
	{
		assert(url[afterDomainPos] == '/');
		url[afterDomainPos] = 0;
		afterDomainPos++;
	}

	printf("Domain: '%s'\nAfter domain: '%s'\n", url+domainPos, url+afterDomainPos);

	char* portString = port80String;

	if (startupNetwork() != 0)
	{
		fprintf(stderr, "WSAStartup() failed\n");
		exit(EXIT_FAILURE);
	}

	struct addrinfo *pAddrInfo;
	struct addrinfo hints;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	int status = getaddrinfo(url+domainPos, portString, &hints, &pAddrInfo);

	if (status != 0)
	{
		fprintf(stderr, "getaddrinfo() failed\n");
		exit(EXIT_FAILURE);
	}

	struct addrinfo * currentAddrInfo;

	SOCKET serverSocket;

	for (currentAddrInfo = pAddrInfo; currentAddrInfo != NULL; currentAddrInfo = currentAddrInfo->ai_next)
	{
		serverSocket = socket(currentAddrInfo->ai_family, currentAddrInfo->ai_socktype, currentAddrInfo->ai_protocol);

		if (serverSocket == -1)
			continue;

		if (connect(serverSocket, currentAddrInfo->ai_addr, currentAddrInfo->ai_addrlen) != -1)
			break;

		closesocket(serverSocket);
	}

	if (currentAddrInfo == NULL)
	{
		fprintf(stderr, "connect() failed\n");
		freeaddrinfo(pAddrInfo);
		exit(EXIT_FAILURE);
	}

	freeaddrinfo(pAddrInfo);

	char requestString0[] = " /";
	char requestString1[] = " HTTP/1.1\r\nHost: ";
	char requestString2[] = "\r\n\r\n";

	send(serverSocket, command, strlen(command), 0);
	send(serverSocket, requestString0, strlen(requestString0), 0);
	send(serverSocket, url+afterDomainPos, strlen(url+afterDomainPos), 0);
	send(serverSocket, requestString1, strlen(requestString1), 0);
	send(serverSocket, url+domainPos, strlen(url+domainPos), 0);
	send(serverSocket, requestString2, strlen(requestString2), 0);

	char buffer[256];

	while (0 < (status = recv(serverSocket, buffer, sizeof(buffer)-1, 0)))
	{
		buffer[status] = 0;
		printf("%s", buffer);
	}

	if (status == SOCKET_ERROR)
	{
		fprintf(stderr, "recv() failed\n");
		closesocket(serverSocket);
		cleanupNetwork();
		exit(EXIT_FAILURE);
	}

	closesocket(serverSocket);
	cleanupNetwork();
	return EXIT_SUCCESS;
}
