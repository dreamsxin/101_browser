#ifndef _OpenGLTextureManager_h
#define _OpenGLTextureManager_h

#include "gui/Texture.h"
#include <windows.h>
#include <gl/gl.h>

GLenum colorModeTextureFormat(ColorMode colorMode);

GLuint generateOpenGLTexture(Texture* in_pTexture);

#endif
