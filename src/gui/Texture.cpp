#include "gui/Texture.h"

size_t colorModePixelBytesCount(ColorMode colorMode)
{
	switch (colorMode)
	{
	case ColorModeInvalid:
		return 0;
	case ColorModeRGB:
		return 3;
	case ColorModeRGBA:
		return 4;
	}
}

size_t textureLineBytesCount(const Texture* in_pTexture)
{
	return (in_pTexture->width * colorModePixelBytesCount(in_pTexture->colorMode)*4+3)/4;
}

size_t textureBytesCount(const Texture* in_pTexture)
{
	return in_pTexture->height * textureLineBytesCount(in_pTexture);
}

void allocateTextureMemory(Texture* in_pTexture)
{
	in_pTexture->data = (unsigned char*) malloc(textureBytesCount(in_pTexture));
}
