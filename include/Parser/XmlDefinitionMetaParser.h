#ifndef _XmlDefinitionMetaParser_h
#define _XmlDefinitionMetaParser_h

#include <list>
#include <string>

struct XmlDefinitionBNF
{
	std::string symbol;
	std::string expression;
};

struct XmlDefinition
{
	std::list<XmlDefinitionBNF> bnfList;
};

#endif
