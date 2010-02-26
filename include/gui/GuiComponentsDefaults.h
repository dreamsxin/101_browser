#ifndef _GuiComponentsDefaults_h
#define _GuiComponentsDefaults_h

#include "gui/Color.h"

namespace Gui
{
	namespace Components
	{
		namespace Defaults
		{
			const float cColorByteToFloat = 1.0f/255.0f;

			const Color4f cColorWhite = {1.0f, 1.0f, 1.0f, 1.0f}; // (255, 255, 255)

			const Color4f cWindowBackgroundColor = {
				236*cColorByteToFloat, 
				233*cColorByteToFloat, 
				216*cColorByteToFloat, 1.0f
			};

			const Color4f cCheckBoxTopLeftBackgroundColor = {
				220*cColorByteToFloat, 
				220*cColorByteToFloat, 
				215*cColorByteToFloat, 1.0f
			};
			const Color4f cCheckBoxTopRightBackgroundColor = {
				241*cColorByteToFloat, 
				241*cColorByteToFloat, 
				239*cColorByteToFloat, 1.0f
			};
			const Color4f cCheckBoxBottomLeftBackgroundColor = cCheckBoxTopRightBackgroundColor;
			const Color4f cCheckBoxBottomRightBackgroundColor = cColorWhite;
			const Color4f cCheckBoxBorderColor = {
				28*cColorByteToFloat, 
				81*cColorByteToFloat, 
				128*cColorByteToFloat, 1.0f
			};
			const Color4f cCheckBoxHookColor = {
				33*cColorByteToFloat, 
				161*cColorByteToFloat, 
				33*cColorByteToFloat, 1.0f
			};


			const Color4f cTextBoxBackgroundColor = cColorWhite;
			const Color4f cTextBoxBorderColor = {
				127*cColorByteToFloat, 
				157*cColorByteToFloat, 
				185*cColorByteToFloat, 1.0f
			};


			// The width and height of the checkbox (without border)
			const unsigned int cCheckBoxWidth = 11;
			const unsigned int cCheckBoxHeight = 11;
			const unsigned int cCheckBoxBorderSize = 1;
			const unsigned int cCheckBoxTextDistance = 7;

			// all the size is 13 - and it gets clipped to this size
			const unsigned int cRadioButtonWidth = 11;
			const unsigned int cRadioButtonHeight = 11;
			const unsigned int cRadioButtonTextDistance = 7;

			// The height of the textbox (without border)
			const unsigned int cTextBoxHeight = 18;
			const unsigned int cTextBoxBorderSize = 1;
		}
	}
}

#endif
