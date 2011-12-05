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
GLenum colorModeInternalFormat(ColorMode colorMode);

void createOpenGLTexture(Texture* in_pTexture);

#endif
