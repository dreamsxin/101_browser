#include <cstdio>       // perror
#include <cstdlib>	// exit
#include <cstring>      // memset
#include <unistd.h>     // close
#include <sys/socket.h> // socket
#include <netinet/in.h> // IPPROTO_TCP
#include <netdb.h>      // getaddrinfo

char GETstring[] = "GET";

int main(int argc, char **argv)
{
	int serverSocket;

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

	int status;
	struct addrinfo *pAddrInfo;
	struct addrinfo hints;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	status = getaddrinfo(hostName, "80", &hints, &pAddrInfo);

	if (status != 0)
	{
		perror("getaddrinfo()");
		exit(EXIT_FAILURE);
	}

	struct addrinfo * currentAddrInfo;

	for (currentAddrInfo = pAddrInfo; currentAddrInfo != NULL; currentAddrInfo = currentAddrInfo->ai_next)
	{
		serverSocket = socket(currentAddrInfo->ai_family, currentAddrInfo->ai_socktype, currentAddrInfo->ai_protocol);

		if (serverSocket == -1)
			continue;

		if (connect(serverSocket, currentAddrInfo->ai_addr, currentAddrInfo->ai_addrlen) != -1)
			break;

		close(serverSocket);
	}

	if (currentAddrInfo == NULL)
	{
		perror("connect()");
		freeaddrinfo(pAddrInfo);
		exit(EXIT_FAILURE);
	}

	freeaddrinfo(pAddrInfo);

	char requestString0[] = " / HTTP/1.1\r\nHost: ";
	char requestString1[] = "\r\n\r\n";

	ssize_t bytesCount;

	write(serverSocket, command, strlen(command));
	write(serverSocket, requestString0, strlen(requestString0));
	write(serverSocket, hostName, strlen(hostName));
	write(serverSocket, requestString1, strlen(requestString1));

	char buffer[256];

	while (0 < (bytesCount = read(serverSocket, buffer, sizeof(buffer)-1)))
	{
		buffer[bytesCount] = 0;
		printf("%s", buffer);
	}

end:
	close(serverSocket);
	return 0;
}

