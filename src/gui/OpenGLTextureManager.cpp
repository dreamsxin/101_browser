#include "gui/OpenGLTextureManager.h"
#include "GL/glext.h" // for GL_CLAMP_TO_EDGE

GLenum colorModeTextureFormat(ColorMode colorMode)
{
	switch (colorMode)
	{
	case ColorModeInvalid:
		return 0;
	case ColorModeRGB:
		return GL_RGB;
	case ColorModeRGBA:
		return GL_RGBA;
	}
}

GLuint createOpenGLTexture(const Texture* in_pTexture)
{
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D,                           // type of texture
		0,                                                // the texture level
		colorModePixelBytesCount(in_pTexture->colorMode), // number of bytes per pixel
		in_pTexture->width,                               // width
		in_pTexture->height,                              // height
		0,                                                // border size
		colorModeTextureFormat(in_pTexture->colorMode),   // the format of the colors
		                                                  // GL_RGB, GL_RGBA etc.
		GL_UNSIGNED_BYTE,                                 // the type of each component
		in_pTexture->data
		);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Linear Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Linear Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);

	return textureID;
}
