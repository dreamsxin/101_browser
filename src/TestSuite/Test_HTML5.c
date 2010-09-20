#include <assert.h>
#include "TestSuite/Tests.h"
#include "TestSuite/TestSuite.h"
#include "HTML5/2_3.h"
#include "HTML5/2_4_1.h"
#include "HTML5/2_4_5.h"
#include "HTML5/ASCIIStringUnicodeIterator.h"
#include "MiniStdlib/MTAx_cstdlib.h"

void test2_3()
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

	ASCIIStringUnicodeIteratorState s0;
	ASCIIStringUnicodeIteratorState s1;

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
}

void test2_4_1()
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

void test2_4_5()
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
	test2_3();
	wprintf(L"Testing 2.4.1\n");
	test2_4_1();
	wprintf(L"Testing 2.4.5\n");
	test2_4_5();
}
