#ifndef _X86_h
#define _X86_h

#ifdef __cplusplus
extern "C" {
#endif
#ifdef _WIN32
__declspec(dllexport)
#endif
int Check_CPU_support_AES();
#ifdef __cplusplus
}
#endif

#endif
