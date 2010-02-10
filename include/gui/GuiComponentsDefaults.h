#ifndef _GuiComponentsDefaults_h
#define _GuiComponentsDefaults_h

#include "gui/Color.h"

const float cColorByteToFloat = 1.0f/255.0f;

const Color4f cColorWhite = {1.0f, 1.0f, 1.0f, 1.0f}; // (255, 255, 255)

const Color4f cDefaultBackgroundColor = {
	236*cColorByteToFloat, 
	233*cColorByteToFloat, 
	216*cColorByteToFloat, 1.0f};


const Color4f cCheckboxTopLeftBackgroundColor = {
	220*cColorByteToFloat, 
	220*cColorByteToFloat, 
	215*cColorByteToFloat, 1.0f};
const Color4f cCheckboxTopRightBottomLeftBackgroundColor = {
	241*cColorByteToFloat, 
	241*cColorByteToFloat, 
	239*cColorByteToFloat, 1.0f}; 
const Color4f cCheckboxBottomRightBackgroundColor = cColorWhite;
const Color4f cCheckboxBorderColor = {
	 28*cColorByteToFloat, 
	 81*cColorByteToFloat, 
	128*cColorByteToFloat, 1.0f};
const Color4f cCheckboxHookColor = {
	 33*cColorByteToFloat, 
	161*cColorByteToFloat, 
	 33*cColorByteToFloat, 1.0f};

const Color4f cTextBoxBackgroundColor = cColorWhite;
const Color4f cTextBoxBorderColor = {
	127*cColorByteToFloat, 
	157*cColorByteToFloat, 
	185*cColorByteToFloat, 1.0f};

#endif