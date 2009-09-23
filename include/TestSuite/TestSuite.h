#ifndef _TestSuite_h
#define _TestSuite_h

#define test(_Expression) (void)( ((!!(_Expression)) && (testPassed(_CRT_WIDE(#_Expression), _CRT_WIDE(__FILE__), __LINE__), 1)) || (testFailed(_CRT_WIDE(#_Expression), _CRT_WIDE(__FILE__), __LINE__), 0) )

void testPassed(const wchar_t * _Message, const wchar_t *_File, unsigned int _Line);
void testFailed(const wchar_t * _Message, const wchar_t *_File, unsigned int _Line);

#endif
