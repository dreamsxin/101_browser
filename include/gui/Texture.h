#ifndef _Texture_h
#define _Texture_h

// for NULL
#include <cstdlib>
#include <windows.h>
#include <gl/gl.h>

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
	GLuint textureID;

	Texture() : width(0), height(0), colorMode(ColorModeInvalid), data(NULL), textureID(0) { }
};

size_t colorModePixelBytesCount(ColorMode colorMode);
size_t textureLineBytesCount(const Texture* in_pTexture);
size_t textureBytesCount(const Texture* in_pTexture);

void allocateTextureMemory(Texture* in_pTexture);
void freeTextureMemory(Texture* in_pTexture);

GLenum colorModeTextureFormat(ColorMode colorMode);

void createOpenGLTexture(Texture* in_pTexture);

#endif
