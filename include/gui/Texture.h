#ifndef _Texture_h
#define _Texture_h

struct Texture
{
	unsigned long width, height;
	unsigned char* data;

	Texture() : width(0), height(0), data(NULL) { }
}

#endif
