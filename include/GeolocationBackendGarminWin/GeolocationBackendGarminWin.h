#ifndef _GeolocationBackendGarminWin_h
#define _GeolocationBackendGarminWin_h

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
__declspec(dllexport)
#endif
HANDLE initializeGeolocationBackendGarmin(DWORD *in_out_pUsbSize);

#ifdef __cplusplus
}
#endif

#endif
