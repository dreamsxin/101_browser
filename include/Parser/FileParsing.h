#ifndef _FileParsing_h
#define _FileParsing_h

#include <cstdio>
#include "Parser/AbstractParser.h"

bool parseFile(FILE* in_file, AbstractParser* in_parser);

#endif
