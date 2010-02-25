#ifndef _Texture_h
#define _Texture_h

#include <cstdlib>

enum ColorMode
{
	ColorModeInvalid, // as a default value
	ColorModeRGB,
	ColorModeRGBA
};

struct Texture
{
	unsigned long width, height;
	ColorMode colorMode;
	unsigned char* data;

	Texture() : width(0), height(0), colorMode(ColorModeInvalid), data(NULL) { }
};

size_t colorModePixelBytesCount(ColorMode colorMode);
size_t textureLineBytesCount(const Texture* in_pTexture);
size_t textureBytesCount(const Texture* in_pTexture);

void allocateTextureMemory(Texture* in_pTexture);

#endif
