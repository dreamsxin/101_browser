#ifndef _ErrorHandling_h
#define _ErrorHandling_h

#include <cstdlib>

#ifdef _WIN32
#include "windows.h"

inline void showErrorMessage(const wchar_t* const in_message)
{
	MessageBox(HWND_DESKTOP, in_message, L"Error", MB_OK | MB_ICONEXCLAMATION);
}

#else

#include <cwchar>
#include <cstdio>

inline void showErrorMessage(const wchar_t* const in_message)
{
	fwprintf(stderr, L"Error: %s\n", in_message);
}

#endif

inline void showErrorMessageAndExit(const wchar_t* const in_message)
{
	showErrorMessage(in_message);
	exit(EXIT_FAILURE);
}

#endif
