#ifndef _XmlDefinition_h
#define _XmlDefinition_h

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
