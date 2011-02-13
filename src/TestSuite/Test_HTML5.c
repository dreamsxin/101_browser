/*
 * Copyright 2008-2011 Wolfgang Keller
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <assert.h>
#include "TestSuite/Tests.h"
#include "TestSuite/TestSuite.h"
#include "HTML5/2_3.h"
#include "HTML5/2_4.h"
#include "HTML5/2_5_1.h"
#include "HTML5/2_5_4_1.h"
#include "HTML5/2_5_5.h"
#include "HTML5/ASCIIStringUnicodeIterator.h"
#include "HTML5/ASCIIStringUnicodeCyclicIterator.h"
#include "MiniStdlib/MTAx_cstdio.h"
#include "BigNumber/BigInteger.h"
#include "IO/MemoryByteStream.h"
#include "IO/PipeStream.h"

void test_2_3()
{
	/* two empty strings */
	char empty0[] = "";
	char empty1[] = "";
	char a[]      = "a";
	char Ab[]     = "Ab";
	char aB[]     = "aB";
	char AbC1230[]   = "AbC123";
	char AbC1231[]   = "AbC123";

	SingleIterator it = asciiStringUnicodeIterator_create();
	SingleIterator itc = asciiStringUnicodeCyclicIterator_create();

	ASCIIStringUnicodeIteratorState s0;
	ASCIIStringUnicodeIteratorState s1;

	ASCIIStringUnicodeCyclicIteratorState cs0;
	ASCIIStringUnicodeCyclicIteratorState cs1;

	s0 = asciiStringUnicodeIteratorState_create(empty0);
	s1 = asciiStringUnicodeIteratorState_create(empty1);
	test(compareStringsCaseSensitive(it, &s0, &s1));
	s0 = asciiStringUnicodeIteratorState_create(empty0);
	s1 = asciiStringUnicodeIteratorState_create(empty1);
	test(compareStringsASCIICaseInsensitive(it, &s0, &s1));
	s0 = asciiStringUnicodeIteratorState_create(empty0);
	s1 = asciiStringUnicodeIteratorState_create(empty1);
	test(prefixMatch(it, &s0, &s1));

	s0 = asciiStringUnicodeIteratorState_create(empty0);
	s1 = asciiStringUnicodeIteratorState_create(a);
	test(!compareStringsCaseSensitive(it, &s0, &s1));
	s0 = asciiStringUnicodeIteratorState_create(empty0);
	s1 = asciiStringUnicodeIteratorState_create(a);
	test(!compareStringsASCIICaseInsensitive(it, &s0, &s1));
	s0 = asciiStringUnicodeIteratorState_create(empty0);
	s1 = asciiStringUnicodeIteratorState_create(a);
	test(prefixMatch(it, &s0, &s1));
	s0 = asciiStringUnicodeIteratorState_create(empty0);
	s1 = asciiStringUnicodeIteratorState_create(a);
	test(!prefixMatch(it, &s1, &s0));

	s0 = asciiStringUnicodeIteratorState_create(Ab);
	s1 = asciiStringUnicodeIteratorState_create(AbC1230);
	test(prefixMatch(it, &s0, &s1));

	s0 = asciiStringUnicodeIteratorState_create(Ab);
	s1 = asciiStringUnicodeIteratorState_create(aB);
	test(!compareStringsCaseSensitive(it, &s0, &s1));
	s0 = asciiStringUnicodeIteratorState_create(Ab);
	s1 = asciiStringUnicodeIteratorState_create(aB);
	test(compareStringsASCIICaseInsensitive(it, &s0, &s1));
	s0 = asciiStringUnicodeIteratorState_create(Ab);
	s1 = asciiStringUnicodeIteratorState_create(aB);
	test(!prefixMatch(it, &s0, &s1));
	s0 = asciiStringUnicodeIteratorState_create(Ab);
	s1 = asciiStringUnicodeIteratorState_create(aB);
	test(!prefixMatch(it, &s1, &s0));

	convertStringToASCIILowercase(AbC1230);
	s0 = asciiStringUnicodeIteratorState_create(AbC1230);
	s1 = asciiStringUnicodeIteratorState_create("abc123");
	test(compareStringsCaseSensitive(it, &s0, &s1));

	convertStringToASCIIUppercase(AbC1231);
	s0 = asciiStringUnicodeIteratorState_create(AbC1231);
	s1 = asciiStringUnicodeIteratorState_create("ABC123");
	test(compareStringsCaseSensitive(it, &s0, &s1));
	
	/*
	 * Test cases whether the functions correctly
	 * handle cyclic iterators
	 */
	cs0 = asciiStringUnicodeCyclicIteratorState_create(a);
	cs1 = asciiStringUnicodeCyclicIteratorState_create(a);
	test(compareStringsCaseSensitive(itc, &cs0, &cs1));

	cs0 = asciiStringUnicodeCyclicIteratorState_create(a);
	cs1 = asciiStringUnicodeCyclicIteratorState_create(a);
	test(compareStringsASCIICaseInsensitive(itc, &cs0, &cs1));

	cs0 = asciiStringUnicodeCyclicIteratorState_create(a);
	cs1 = asciiStringUnicodeCyclicIteratorState_create(a);
	test(prefixMatch(itc, &cs0, &cs1));
}

typedef struct
{
	uint8_t *pInputBytes;
	MemoryByteStreamReadState *pReadState;
} Test_2_4_Userdata;

void test_2_4_Coroutine(PipeStreamState *in_pPipeStreamState, void *in_pUserData)
{
	ByteStreamWriteInterface pipeStreamWriteInterface = { &pipeStreamWrite };

	Test_2_4_Userdata *pUserData = (Test_2_4_Userdata *) in_pUserData;

	convertUTF8toCodepoints(cMemoryStreamReadInterface, pUserData->pReadState, 
		pipeStreamWriteInterface, in_pPipeStreamState);
}

void test_2_4()
{
	Test_2_4_Userdata test_2_4_userdata;
	bool result;
	MemoryByteStreamReadState readState;
	PipeStreamState pipeStreamState;
	CoroutineDescriptor thisCoroutine;
	CoroutineDescriptor otherCoroutine;
	size_t idx;
	UnicodeCodePoint currentCodePoint;
	size_t readCount;

	result = convertThreadToCoroutine(&thisCoroutine);
	test(result);

	if (!result)
		return;

	{
		/*
		 * the test string from
		 * http://www.whatwg.org/specs/web-apps/current-work/multipage/infrastructure.html#utf-8
		 * (12 February 2011)
		 */
		uint8_t inputBytes[] = { 0x41, 0x98, 0xBA, 0x42, 0xE2, 0x98, 0x43, 0xE2, 0x98, 0xBA, 0xE2, 0x98 };
		UnicodeCodePoint outputCodepoints[] = { 'A', 0xFFFD, 0xFFFD, 'B', 0xFFFD, 'C', 0x263A, 0xFFFD };

		initMemoryByteStreamReadState(&readState, inputBytes, sizeof(inputBytes));

		test_2_4_userdata.pInputBytes = inputBytes;
		test_2_4_userdata.pReadState = &readState;

		result = initPipeStreamState(&pipeStreamState, false, &thisCoroutine, &otherCoroutine, 
			&test_2_4_Coroutine, &test_2_4_userdata);
		test(result);

		if (!result)
			return;

		for (idx = 0; idx < sizeof(outputCodepoints) / sizeof(UnicodeCodePoint); idx++)
		{
			readCount = pipeStreamRead(&pipeStreamState, &currentCodePoint, sizeof(UnicodeCodePoint));

			test(readCount == sizeof(UnicodeCodePoint));
			test(currentCodePoint == outputCodepoints[idx]);
		}

		readCount = pipeStreamRead(&pipeStreamState, &currentCodePoint, sizeof(UnicodeCodePoint));
		test(0 == readCount);

		deletePipeStreamState(&pipeStreamState);
	}
}

void test_2_5_1()
{
	{
		test(!isSpaceCharacter(0x8));
		assert('\t' == 0x9);
		test( isSpaceCharacter('\t'));
		assert('\n' == 0xA);
		test( isSpaceCharacter('\n'));
		assert('\v' == 0xB);
		test(!isSpaceCharacter('\v'));
		assert('\f' == 0xC);
		test( isSpaceCharacter('\f'));
		assert('\r' == 0xD);
		test( isSpaceCharacter('\r'));
		test(!isSpaceCharacter(0xE));

		test(!isSpaceCharacter(0x1F));
		test( isSpaceCharacter(' '));
		assert('!' == 0x21);
		test(!isSpaceCharacter('!'));
	}

	{
		test(!isAlphanumericASCIICharacter('0'-1));
		test( isAlphanumericASCIICharacter('0'));
		test( isAlphanumericASCIICharacter('5'));
		test( isAlphanumericASCIICharacter('9'));
		test(!isAlphanumericASCIICharacter('9'+1));

		test(!isAlphanumericASCIICharacter('A'-1));
		test( isAlphanumericASCIICharacter('A'));
		test( isAlphanumericASCIICharacter('M'));
		test( isAlphanumericASCIICharacter('Z'));
		test(!isAlphanumericASCIICharacter('Z'+1));

		test(!isAlphanumericASCIICharacter('a'-1));
		test( isAlphanumericASCIICharacter('a'));
		test( isAlphanumericASCIICharacter('m'));
		test( isAlphanumericASCIICharacter('z'));
		test(!isAlphanumericASCIICharacter('z'+1));
	}

	{
		FILE* propListFile = MTAx_fopen("data/Unicode/PropList.txt", "rb");
		void* intervals;
		size_t whiteSpaceIntervalsCount;

		bool result = allocWhite_SpaceCharacterIntervals(propListFile, &intervals, &whiteSpaceIntervalsCount);
		test(result);
		test(whiteSpaceIntervalsCount > 0);
		MTAx_fclose(&propListFile);

		{
			test(!isWhite_SpaceCharacter(0x8, intervals, whiteSpaceIntervalsCount));
			assert('\t' == 0x9);
			test(isWhite_SpaceCharacter('\t', intervals, whiteSpaceIntervalsCount));
			assert('\r' == 0xD);
			test(isWhite_SpaceCharacter('\r', intervals, whiteSpaceIntervalsCount));

			test(!isWhite_SpaceCharacter(0x1F, intervals, whiteSpaceIntervalsCount));
			test(isWhite_SpaceCharacter(' ', intervals, whiteSpaceIntervalsCount));
			assert('!' == 0x21);
			test(!isWhite_SpaceCharacter('!', intervals, whiteSpaceIntervalsCount));

			test(!isWhite_SpaceCharacter('a', intervals, whiteSpaceIntervalsCount));

			test( isWhite_SpaceCharacter(0x85, intervals, whiteSpaceIntervalsCount));

			test(!isWhite_SpaceCharacter(0x9F, intervals, whiteSpaceIntervalsCount));
			test( isWhite_SpaceCharacter(0xA0, intervals, whiteSpaceIntervalsCount));
			test(!isWhite_SpaceCharacter(0xA1, intervals, whiteSpaceIntervalsCount));

			test(!isWhite_SpaceCharacter(0x167F, intervals, whiteSpaceIntervalsCount));
			test( isWhite_SpaceCharacter(0x1680, intervals, whiteSpaceIntervalsCount));
			test(!isWhite_SpaceCharacter(0x1681, intervals, whiteSpaceIntervalsCount));

			test(!isWhite_SpaceCharacter(0x180D, intervals, whiteSpaceIntervalsCount));
			test( isWhite_SpaceCharacter(0x180E, intervals, whiteSpaceIntervalsCount));
			test(!isWhite_SpaceCharacter(0x180F, intervals, whiteSpaceIntervalsCount));

			test(!isWhite_SpaceCharacter(0x1FFF, intervals, whiteSpaceIntervalsCount));
			test( isWhite_SpaceCharacter(0x2000, intervals, whiteSpaceIntervalsCount));
			test( isWhite_SpaceCharacter(0x2005, intervals, whiteSpaceIntervalsCount));
			test( isWhite_SpaceCharacter(0x200A, intervals, whiteSpaceIntervalsCount));
			test(!isWhite_SpaceCharacter(0x200B, intervals, whiteSpaceIntervalsCount));

			test(!isWhite_SpaceCharacter(0x2027, intervals, whiteSpaceIntervalsCount));
			test( isWhite_SpaceCharacter(0x2028, intervals, whiteSpaceIntervalsCount));
			test( isWhite_SpaceCharacter(0x2029, intervals, whiteSpaceIntervalsCount));
			test(!isWhite_SpaceCharacter(0x202A, intervals, whiteSpaceIntervalsCount));

			test(!isWhite_SpaceCharacter(0x202E, intervals, whiteSpaceIntervalsCount));
			test( isWhite_SpaceCharacter(0x202F, intervals, whiteSpaceIntervalsCount));
			test(!isWhite_SpaceCharacter(0x2030, intervals, whiteSpaceIntervalsCount));

			test(!isWhite_SpaceCharacter(0x205E, intervals, whiteSpaceIntervalsCount));
			test( isWhite_SpaceCharacter(0x205F, intervals, whiteSpaceIntervalsCount));
			test(!isWhite_SpaceCharacter(0x2060, intervals, whiteSpaceIntervalsCount));

			test(!isWhite_SpaceCharacter(0x2FFF, intervals, whiteSpaceIntervalsCount));
			test( isWhite_SpaceCharacter(0x3000, intervals, whiteSpaceIntervalsCount));
			test(!isWhite_SpaceCharacter(0x3001, intervals, whiteSpaceIntervalsCount));
		}

		{
			char empty[]     = ""     ;
			char space[]     = " "    ;
			char A[]         = "A"    ;
			char spaceA[]    = " A"   ;
			char tabSpace[]  = "\t "  ;
			char tabSpaceA[] = "\t A" ;
			char tabVert[]   = "\t\v" ;
			char tabVertA[]  = "\t\vA";
			char vertTab[]   = "\v\t" ;
			char vertTabA[]  = "\v\tA";
			void* pCurrentDatum;

			SingleIterator it = asciiStringUnicodeIterator_create();
			ASCIIStringUnicodeIteratorState s;
			ASCIIStringUnicodeIteratorState s0;
			ASCIIStringUnicodeIteratorState s1;

			s = asciiStringUnicodeIteratorState_create(empty);
			s0 = s;
			s1 = s;
			test(!skipWhitespace(it, &s0));
			test((*it.mpfGet)(&s0) == NULL);
			test(!skipWhite_SpaceCharacters(it, &s1, intervals, whiteSpaceIntervalsCount));
			test((*it.mpfGet)(&s1) == NULL);

			s = asciiStringUnicodeIteratorState_create(space);
			s0 = s;
			s1 = s;
			test(!skipWhitespace(it, &s0));
			test((*it.mpfGet)(&s0) == NULL);
			test(!skipWhite_SpaceCharacters(it, &s1, intervals, whiteSpaceIntervalsCount));
			test((*it.mpfGet)(&s1) == NULL);

			s = asciiStringUnicodeIteratorState_create(A);
			s0 = s;
			s1 = s;
			test(skipWhitespace(it, &s0));
			pCurrentDatum = (*it.mpfGet)(&s0);
			test(pCurrentDatum != NULL);
			if (pCurrentDatum != NULL)
			{
				test(*((UnicodeCodePoint*) pCurrentDatum) == 'A');
			}
			test(skipWhite_SpaceCharacters(it, &s1, intervals, whiteSpaceIntervalsCount));
			pCurrentDatum = (*it.mpfGet)(&s1);
			test(pCurrentDatum != NULL);
			if (pCurrentDatum != NULL)
			{
				test(*((UnicodeCodePoint*) pCurrentDatum) == 'A');
			}

			s = asciiStringUnicodeIteratorState_create(spaceA);
			s0 = s;
			s1 = s;
			test(skipWhitespace(it, &s0));
			pCurrentDatum = (*it.mpfGet)(&s0);
			test(pCurrentDatum != NULL);
			if (pCurrentDatum != NULL)
			{
				test(*((UnicodeCodePoint*) pCurrentDatum) == 'A');
			}
			test(skipWhite_SpaceCharacters(it, &s1, intervals, whiteSpaceIntervalsCount));
			pCurrentDatum = (*it.mpfGet)(&s1);
			test(pCurrentDatum != NULL);
			if (pCurrentDatum != NULL)
			{
				test(*((UnicodeCodePoint*) pCurrentDatum) == 'A');
			}

			s = asciiStringUnicodeIteratorState_create(tabSpace);
			s0 = s;
			s1 = s;
			test(!skipWhitespace(it, &s0));
			test((*it.mpfGet)(&s0) == NULL);
			test(!skipWhite_SpaceCharacters(it, &s1, intervals, whiteSpaceIntervalsCount));
			test((*it.mpfGet)(&s1) == NULL);

			s = asciiStringUnicodeIteratorState_create(tabSpaceA);
			s0 = s;
			s1 = s;
			test(skipWhitespace(it, &s0));
			pCurrentDatum = (*it.mpfGet)(&s0);
			test(pCurrentDatum != NULL);
			if (pCurrentDatum != NULL)
			{
				test(*((UnicodeCodePoint*) pCurrentDatum) == 'A');
			}
			test(skipWhite_SpaceCharacters(it, &s1, intervals, whiteSpaceIntervalsCount));
			pCurrentDatum = (*it.mpfGet)(&s1);
			test(pCurrentDatum != NULL);
			if (pCurrentDatum != NULL)
			{
				test(*((UnicodeCodePoint*) pCurrentDatum) == 'A');
			}

			s = asciiStringUnicodeIteratorState_create(tabVert);
			s0 = s;
			s1 = s;
			test(skipWhitespace(it, &s0));
			pCurrentDatum = (*it.mpfGet)(&s0);
			test(pCurrentDatum != NULL);
			if (pCurrentDatum != NULL)
			{
				test(*((UnicodeCodePoint*) pCurrentDatum) == '\v');
			}
			test(!skipWhite_SpaceCharacters(it, &s1, intervals, whiteSpaceIntervalsCount));
			test((*it.mpfGet)(&s1) == NULL);

			s = asciiStringUnicodeIteratorState_create(tabVertA);
			s0 = s;
			s1 = s;
			test(skipWhitespace(it, &s0));
			pCurrentDatum = (*it.mpfGet)(&s0);
			test(pCurrentDatum != NULL);
			if (pCurrentDatum != NULL)
			{
				test(*((UnicodeCodePoint*) pCurrentDatum) == '\v');
			}
			test(skipWhite_SpaceCharacters(it, &s1, intervals, whiteSpaceIntervalsCount));
			pCurrentDatum = (*it.mpfGet)(&s1);
			test(pCurrentDatum != NULL);
			if (pCurrentDatum != NULL)
			{
				test(*((UnicodeCodePoint*) pCurrentDatum) == 'A');
			}

			s = asciiStringUnicodeIteratorState_create(vertTab);
			s0 = s;
			s1 = s;
			test(skipWhitespace(it, &s0));
			pCurrentDatum = (*it.mpfGet)(&s0);
			test(pCurrentDatum != NULL);
			if (pCurrentDatum != NULL)
			{
				test(*((UnicodeCodePoint*) pCurrentDatum) == '\v');
			}
			test(!skipWhite_SpaceCharacters(it, &s1, intervals, whiteSpaceIntervalsCount));
			test((*it.mpfGet)(&s1) == NULL);

			s = asciiStringUnicodeIteratorState_create(vertTabA);
			s0 = s;
			s1 = s;
			test(skipWhitespace(it, &s0));
			pCurrentDatum = (*it.mpfGet)(&s0);
			test(pCurrentDatum != NULL);
			if (pCurrentDatum != NULL)
			{
				test(*((UnicodeCodePoint*) pCurrentDatum) == '\v');
			}
			test(skipWhite_SpaceCharacters(it, &s1, intervals, whiteSpaceIntervalsCount));
			pCurrentDatum = (*it.mpfGet)(&s1);
			test(pCurrentDatum != NULL);
			if (pCurrentDatum != NULL)
			{
				test(*((UnicodeCodePoint*) pCurrentDatum) == 'A');
			}
		}

		freeWhite_SpaceCharacterIntervals(&intervals);
	}
}

void test_2_5_4_1()
{
	char emptyFail[]         = "";
	char tabsFail[]          = "\t\t\t";
	char plusFail[]          = "+";
	char dollarFail[]        = "$";
	char tabsPlusFail[]      = "\t\t\t+";
	char tabsPlus0Pass[]     = "\t\t\t+0";
	char plus9Pass[]         = "+9";
	char plus0DollarFail[]   = "+0$";
	char tabsPlus9PlusFail[] = "\t\t\t+9+";
	char overUint32Pass[] = "00000069876543210";
	// 2^1024-1
	char plusVeryLargePass[] = "179769313486231590772930519078902473361797697894230657273430081157732675805500963132708477322407536021120113879871393357658789768814416622492847430639474124377767893424865485276302219601246094119453082952085005768838150682342462881473913110540827237163350510684586298239947245938479716304835356329624224137215";

	SingleIterator it = asciiStringUnicodeIterator_create();
	ASCIIStringUnicodeIteratorState s;
	UnsignedBigInteger number;

	s = asciiStringUnicodeIteratorState_create(emptyFail);
	test(parseNonNegativeInteger(it, &s, &number) == ResultError);

	s = asciiStringUnicodeIteratorState_create(tabsFail);
	test(parseNonNegativeInteger(it, &s, &number) == ResultError);

	s = asciiStringUnicodeIteratorState_create(plusFail);
	test(parseNonNegativeInteger(it, &s, &number) == ResultError);

	s = asciiStringUnicodeIteratorState_create(dollarFail);
	test(parseNonNegativeInteger(it, &s, &number) == ResultError);

	s = asciiStringUnicodeIteratorState_create(tabsPlusFail);
	test(parseNonNegativeInteger(it, &s, &number) == ResultError);

	s = asciiStringUnicodeIteratorState_create(tabsPlus0Pass);
	test(parseNonNegativeInteger(it, &s, &number) == ResultOK);
	test(number.numberSize == 0);
	freeUnsignedBigInteger(&number);

	s = asciiStringUnicodeIteratorState_create(plus9Pass);
	test(parseNonNegativeInteger(it, &s, &number) == ResultOK);
	test(number.numberSize == 1);
	if (number.numberSize == 1)
	{
		test(number.limbs[0] == 9);
	}
	freeUnsignedBigInteger(&number);

	s = asciiStringUnicodeIteratorState_create(plus0DollarFail);
	test(parseNonNegativeInteger(it, &s, &number) == ResultError);

	s = asciiStringUnicodeIteratorState_create(tabsPlus9PlusFail);
	test(parseNonNegativeInteger(it, &s, &number) == ResultError);

	s = asciiStringUnicodeIteratorState_create(overUint32Pass);
	test(parseNonNegativeInteger(it, &s, &number) == ResultOK);
	test(number.numberSize == 2);
	if (number.numberSize == 2)
	{
		test(number.limbs[0] == 1157066474);
		test(number.limbs[1] == 16);
	}
	freeUnsignedBigInteger(&number);

	s = asciiStringUnicodeIteratorState_create(plusVeryLargePass);
	test(parseNonNegativeInteger(it, &s, &number) == ResultOK);
	test(number.numberSize == 0x20);
	if (number.numberSize == 0x20)
	{
		unsigned int i;
		for (i=0; i<0x20; i++)
		{
			test(number.limbs[i] == 0xFFFFFFFF);
		}
	}
	freeUnsignedBigInteger(&number);
}

void test_2_5_5()
{
	UnsignedBigInteger year;

	test(initUnsignedBigIntegerUC(&year, 1900));
	test(numberOfDaysInMonthOfYear(2, year) == 28);
	freeUnsignedBigInteger(&year);

	test(initUnsignedBigIntegerUC(&year, 1999));
	test(numberOfDaysInMonthOfYear(1, year) == 31);
	test(numberOfDaysInMonthOfYear(2, year) == 28);
	test(numberOfDaysInMonthOfYear(3, year) == 31);
	test(numberOfDaysInMonthOfYear(4, year) == 30);
	test(numberOfDaysInMonthOfYear(5, year) == 31);
	test(numberOfDaysInMonthOfYear(6, year) == 30);
	test(numberOfDaysInMonthOfYear(7, year) == 31);
	test(numberOfDaysInMonthOfYear(8, year) == 31);
	test(numberOfDaysInMonthOfYear(9, year) == 30);
	test(numberOfDaysInMonthOfYear(10, year) == 31);
	test(numberOfDaysInMonthOfYear(11, year) == 30);
	test(numberOfDaysInMonthOfYear(12, year) == 31);
	freeUnsignedBigInteger(&year);

	test(initUnsignedBigIntegerUC(&year, 2000));
	test(numberOfDaysInMonthOfYear(2, year) == 29);
	freeUnsignedBigInteger(&year);

	test(initUnsignedBigIntegerUC(&year, 2004));
	test(numberOfDaysInMonthOfYear(2, year) == 29);
	freeUnsignedBigInteger(&year);

	assert('/' < '0');
	test(!isDigit('/'));
	test(isDigit('0'));
	test(isDigit('5'));
	test(isDigit('9'));
	assert(':' > '9');
	test(!isDigit(':'));
}

void testHTML5()
{
	wprintf(L"Testing 2.3\n");
	test_2_3();
	wprintf(L"Testing 2.4\n");
	test_2_4();
	wprintf(L"Testing 2.5.1\n");
	test_2_5_1();
	wprintf(L"Testing 2.5.4.1\n");
	test_2_5_4_1();
	wprintf(L"Testing 2.5.5\n");
	test_2_5_5();
}
