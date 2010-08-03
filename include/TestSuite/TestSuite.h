#ifndef _TestSuite_h
#define _TestSuite_h

#ifdef __cplusplus
#include <cwchar>
extern "C" {
#else
#include <wchar.h>
#endif

#ifndef _WIN32
/* we really need 2 macros here since else _CRT_WIDE(__FILE__) will be expanded to L__FILE__ */
#define __CRT_WIDE(x) L ## x
#define _CRT_WIDE(x) __CRT_WIDE(x)
#endif

#define test(_Expression) (void)( ((!!(_Expression)) && (testPassed(_CRT_WIDE(#_Expression), _CRT_WIDE(__FILE__), __LINE__), 1)) || (testFailed(_CRT_WIDE(#_Expression), _CRT_WIDE(__FILE__), __LINE__), 0) )

void testPassed(const wchar_t * _Message, const wchar_t *_File, unsigned int _Line);
void testFailed(const wchar_t * _Message, const wchar_t *_File, unsigned int _Line);

#ifdef __cplusplus
}
#endif

#endif
