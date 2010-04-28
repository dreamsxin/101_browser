#include <cstdio>
#include <cstring>
#include <winsock2.h>
#include <ws2tcpip.h>


char GETstring[] = "GET";

int main(int argc, char** argv)
{
	if (argc <= 1)
	{
		printf("Usage: %s [COMMAND] url\n", argv[0]);
		exit(0);
	}

	char *hostName = argv[argc-1];
	char *command;

	if (argc > 2)
	{
		command = argv[1];
	}
	else
	{
		command = GETstring;
	}

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

	status = getaddrinfo(hostName, "80", &hints, &pAddrInfo);

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

	char requestString0[] = " / HTTP/1.1\r\nHost: ";
	char requestString1[] = "\r\n\r\n";

	send(serverSocket, command, strlen(command), 0);
	send(serverSocket, requestString0, strlen(requestString0), 0);
	send(serverSocket, hostName, strlen(hostName), 0);
	send(serverSocket, requestString1, strlen(requestString1), 0);

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
		WSACleanup();
		exit(EXIT_FAILURE);
	}

	closesocket(serverSocket);
	WSACleanup();
	return 0;
}
