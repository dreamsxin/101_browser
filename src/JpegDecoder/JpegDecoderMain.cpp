#include <cstdio>
#include <cstdlib>

bool isStandaloneMarker(unsigned char in_marker)
{
	switch (in_marker)
	{
	case 0x01:
	case 0xD0:
	case 0xD1:
	case 0xD2:
	case 0xD3:
	case 0xD4:
	case 0xD5:
	case 0xD6:
	case 0xD7:
	case 0xD8:
	case 0xD9:
		return true;
	default:
		return false;
	}
}

unsigned short flipBytes(unsigned short in_value)
{
	unsigned char lo = in_value & 0xFF;
	unsigned char hi = in_value >> 8;
	return (lo<<8)+hi;
}

void printSegmentName(unsigned char in_marker)
{
	switch (in_marker)
	{
	case 0xC0:
		printf("SOF_0");
		break;
	case 0xC4:
		printf("DHT");
		break;
	case 0xD8:
		printf("SOI");
		break;
	case 0xD9:
		printf("EOI");
		break;
	case 0xDA:
		printf("SOS");
		break;
	case 0xDB:
		printf("DQT");
		break;
	case 0xDD:
		printf("DRI");
		break;
	case 0xE0:
		printf("APP_0");
		break;
	case 0xFE:
		printf("COM");
		break;
	default:
		printf("???");
		break;
	}
}

int main()
{
	char filename[] = //"testfiles/JPEG_example_JPG_RIP_100.jpg";
		"testfiles/T83/A1.jpg";
	FILE* jpegFile = fopen(filename, "rb");

	if (!jpegFile)
	{
		fprintf(stderr, "Could not open file %s\n", filename);
		exit(1);
	}

	unsigned char currentToken;

	while (true)
	{
		if (fread(&currentToken, 1, 1, jpegFile)!=1)
		{
			break;
		}

		if (currentToken != 0xFF)
		{
			fprintf(stderr, "Expected token FF but received %2X\n", (unsigned int) currentToken);
			break;
		}

		// Skip all 0xFF
		while (currentToken == 0xFF)
		{
			if (fread(&currentToken, 1, 1, jpegFile)!=1)
			{
				fprintf(stderr, "Expected token but received EOF\n");
				break;
			}
		}
		
		printf("FF %2X (", currentToken);
		printSegmentName(currentToken);
		printf(")");

		/*if (currentToken == 0xDA) // SOS - start of scan
		{
			unsigned short Ls; // length of scan
			unsigned char Ns;

			if (fread(&Ls, sizeof(Ls), 1, jpegFile)!=1)
			{
				fprintf(stderr, "Expected Ls but received EOF\n");
				break;
			}

			if (fread(&Ns, sizeof(Ns), 1, jpegFile)!=1)
			{
				fprintf(stderr, "Expected Ns but received EOF\n");
				break;
			}


		} else */
		if (!isStandaloneMarker(currentToken))
		{
			unsigned short length;

			if (fread(&length, 2, 1, jpegFile)!=1)
			{
				fprintf(stderr, "Expected length parameter but received EOF\n");
				break;
			}

			length = flipBytes(length);

			printf(" length=%u", length);

			if (length<2)
			{
				fprintf(stderr, "Expected a length of at least 2\n");
				break;
			}

			unsigned char* data = (unsigned char*) malloc(length-2);

			if (!data)
			{
				fprintf(stderr, "Error allocating data\n");
				break;
			}

			if (fread(data, length-2, 1, jpegFile)!=1)
			{
				fprintf(stderr, "Expected data of length %u but received EOF\n", length-2);
				break;
			}

			free(data);
			data = NULL;
		}

		printf("\n");
	}

	fclose(jpegFile);
}
