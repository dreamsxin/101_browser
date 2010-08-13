#include <assert.h>
#include "TestSuite/Tests.h"
#include "TestSuite/TestSuite.h"
#include "HTML5/2_3.h"
#include "HTML5/2_4_5.h"
#include "HTML5/ASCIIStringUnicodeIterator.h"

void testHTML5()
{
	/* 2.3 */
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

	/* 2.4.5 */
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
}
