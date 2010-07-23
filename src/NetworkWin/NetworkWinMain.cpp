#include <cstdio>
#include <cstring>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "CryptoWin/CryptoWin.h"
#include "Crypto/TLS.h"
#include <ctime>
#include "BasicDataStructures/Endianess.h"

char GETstring[] = "GET";

void printUsageAndExit(char* argv0)
{
	printf(
		"Usage: %s [COMMAND] protocol url\n"
		"Where protocol is http|https\n", argv0);
	exit(0);
}

char port80String[] = "80";
char port443String[] = "443";

int main(int argc, char** argv)
{
	if (argc <= 2)
	{
		printUsageAndExit(argv[0]);
	}

	char *hostName = argv[argc-1];
	char *protocol = argv[argc-2];
	char *command;

	if (argc > 3)
	{
		command = argv[1];
	}
	else
	{
		command = GETstring;
	}

	bool useHttps;
	
	if (strcmp("http", protocol) == 0)
	{
		useHttps = false;
	}
	else if (strcmp("https", protocol) == 0)
	{
		useHttps = true;
	}
	else
	{
		printUsageAndExit(argv[0]);
	}

	char* portString = useHttps ? port443String : port80String;

	WORD wVersionRequired = MAKEWORD(2, 2);
	WSADATA lWSAData;
	int status;

	status = WSAStartup(wVersionRequired, &lWSAData);

	if (status != 0)
	{
		fprintf(stderr, "WSAStartup() failed\n");
		exit(EXIT_FAILURE);
	}

	struct addrinfo *pAddrInfo;
	struct addrinfo hints;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	status = getaddrinfo(hostName, portString, &hints, &pAddrInfo);

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

	if (!useHttps)
	{
		char requestString0[] = " / HTTP/1.1\r\nHost: ";
		char requestString1[] = "\r\n\r\n";

		send(serverSocket, command, strlen(command), 0);
		send(serverSocket, requestString0, strlen(requestString0), 0);
		send(serverSocket, hostName, strlen(hostName), 0);
		send(serverSocket, requestString1, strlen(requestString1), 0);
	}
	else
	{
		MTAx::TLS::TLSPlaintextHeader tlsPlaintextHeader;
		tlsPlaintextHeader.type = MTAx::TLS::ContentType_handshake;
		tlsPlaintextHeader.version = MTAx::TLS::TLS_1_2_ProtocolVersion;
		
		MTAx::Endianess::switchEndianess(&tlsPlaintextHeader.length);

		MTAx::TLS::Random tlsRandom;
		tlsRandom.gmt_unix_time = static_cast<uint32_t>(time(NULL));
		
		MTAx::Crypto::CryptoContext randomNumberContext = MTAx::Crypto::allocRandomNumberContext();
		MTAx::Crypto::getRandomBytes(randomNumberContext, 
			sizeof(tlsRandom.random_bytes), &tlsRandom.random_bytes);
		MTAx::Crypto::freeRandomNumberContext(randomNumberContext);


	}

	char buffer[256];

	while (0 < (status = recv(serverSocket, buffer, sizeof(buffer)-1, 0)))
	{
		if (!useHttps)
		{
			buffer[status] = 0;
			printf("%s", buffer);
		}
		else
		{
			for (size_t currentElement=0; currentElement<(unsigned int) status; currentElement++)
			{
				printf("%2hu ", (unsigned short) buffer[currentElement]);
			}
		}
	}

	if (status == SOCKET_ERROR)
	{
		fprintf(stderr, "recv() failed\n");
		closesocket(serverSocket);
		WSACleanup();
		exit(EXIT_FAILURE);
	}

	closesocket(serverSocket);
	WSACleanup();
	return 0;
}
