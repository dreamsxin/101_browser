#ifndef _GuiOpenGLState_h
#define _GuiOpenGLState_h

#ifdef _WIN32
#include "gui/MultiMouse.h"
#endif

void ReshapeGL(int width, int height);
void initializeOpenGLGuiState();
void UpdateGuiState();
void drawGui(
#ifdef _WIN32
			 const ArrayBlock<Gui::Mouse::RawMouse>* in_pRawMice
#endif
			 );

#endif
