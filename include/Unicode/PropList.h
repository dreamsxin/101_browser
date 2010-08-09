#ifndef _PropList_h
#define _PropList_h

#ifdef __cplusplus
#include <cstdlib>
#else
#include <stdlib.h>
#endif

#include "MiniStdlib/cstdbool.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
__declspec(dllexport)
#endif
bool readPropList(FILE* in_file, char* in_property);

#ifdef __cplusplus
}
#endif

#endif
