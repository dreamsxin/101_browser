#include <cstdio>
#include <winsock2.h>

char GETstring[] = "GET";

int main(int argc, char** argv)
{
	WORD wVersionRequired = MAKEWORD(2, 2);
	WSADATA lWSAData;
	int status;

	status = WSAStartup(wVersionRequired, &lWSAData);

	if (status != 0)
	{
		fprintf(stderr, "WSAStartup() failed.\n");
		exit(EXIT_FAILURE);
	}

	SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (serverSocket == INVALID_SOCKET)
	{
		fprintf(stderr, "socket() failed.\n");
		WSACleanup();
		exit(EXIT_FAILURE);
	}

	WSACleanup();
}
