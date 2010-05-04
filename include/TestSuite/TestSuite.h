#ifndef _TestSuite_h
#define _TestSuite_h

#ifdef _WIN32

#define test(_Expression) (void)( ((!!(_Expression)) && (testPassed(_CRT_WIDE(#_Expression), _CRT_WIDE(__FILE__), __LINE__), 1)) || (testFailed(_CRT_WIDE(#_Expression), _CRT_WIDE(__FILE__), __LINE__), 0) )

#else

// we really need 2 macros here since else WIDEN(__FILE__) will be expanded to L__FILE__
#define WIDEN2(x) L##x
#define WIDEN(x) WIDEN2(x)

#define test(_Expression) (void)( ((!!(_Expression)) && (testPassed(WIDEN(#_Expression), WIDEN(__FILE__), __LINE__), 1)) || (testFailed(WIDEN(#_Expression), WIDEN(__FILE__), __LINE__), 0) )
#endif

void testPassed(const wchar_t * _Message, const wchar_t *_File, unsigned int _Line);
void testFailed(const wchar_t * _Message, const wchar_t *_File, unsigned int _Line);

#endif
