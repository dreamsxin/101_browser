#ifndef _GeolocationBackendGarminWinFrontend_h
#define _GeolocationBackendGarminWinFrontend_h

#include "GeolocationBackendGarminWin/GeolocationBackendGarminWinCommon.h"
#include <windows.h>
#include "MiniStdlib/cstdbool.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
__declspec(dllexport)
#endif
HANDLE initializeGeolocationBackendGarmin(WORD *in_out_pUsbSize);

#ifdef _WIN32
__declspec(dllexport)
#endif
ReceivePacketState getInitialReceivePacketState();

#ifdef _WIN32
__declspec(dllexport)
#endif
bool startSession(ReceivePacketState *in_pState,
				  HANDLE in_garminHandle,
				  WORD in_usbPacketSize);

#ifdef __cplusplus
}
#endif

#endif
