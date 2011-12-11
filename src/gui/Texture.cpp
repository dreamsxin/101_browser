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

#include "gui/Texture.h"
// for GL_CLAMP_TO_EDGE
#include "GL/glext.h"

size_t colorModePixelBytesCount(ColorMode colorMode)
{
	switch (colorMode)
	{
	case ColorModeRGB:
		return 3;
	case ColorModeRGBA:
		return 4;
	case ColorModeInvalid:
	default:
		return 0;
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

void freeTextureMemory(Texture* in_pTexture)
{
	free(in_pTexture->data);
	in_pTexture->data = NULL;
}

GLenum colorModeTextureFormat(ColorMode colorMode)
{
	switch (colorMode)
	{
	case ColorModeRGB:
		return GL_RGB;
	case ColorModeRGBA:
		return GL_RGBA;
	case ColorModeInvalid:
	default:
		return 0;
	}
}

GLenum colorModeInternalFormat(ColorMode colorMode)
{
	switch (colorMode)
	{
	case ColorModeRGB:
		return GL_RGB8;
	case ColorModeRGBA:
		return GL_RGBA8;
	case ColorModeInvalid:
	default:
		return 0;
	}
}

void createOpenGLTexture(Texture* in_pTexture)
{
	glGenTextures(1, &in_pTexture->textureID);
	glBindTexture(GL_TEXTURE_2D, in_pTexture->textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Linear Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Linear Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D,	                          // type of texture
		0,                                                // the texture level
		/*
		* Q: Why do we use GL_RGB8/GL_RGBA8 (this is what colorModeInternalFormat
		*    returns) instead of 3 or 4?
		*
		* A: This is highly recommended on
		*    http://www.opengl.org/wiki/Common_Mistakes#Image_precision
		*/
		colorModeInternalFormat(in_pTexture->colorMode),
		in_pTexture->width,                               // width
		in_pTexture->height,                              // height
		0,                                                // border size
		colorModeTextureFormat(in_pTexture->colorMode),   // the format of the colors
		                                                  // GL_RGB, GL_RGBA etc.
		GL_UNSIGNED_BYTE,                                 // the type of each component
		in_pTexture->data);
}
