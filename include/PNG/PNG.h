#ifndef _PNG_h
#define _PNG_h

#include "Util/ReadResult.h"
#include "MiniStdlib/MTAx_cstdio.h"
#include "MiniStdlib/cstdint.h"
#include "MiniStdlib/cstdbool.h"

#ifdef _WIN32
__declspec(dllexport)
#endif
ReadResult read_PNG(FILE* in_pngFile);

#endif
