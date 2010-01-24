#ifndef _SingleChain_h
#define _SingleChain_h

#include <cassert>

template<typename T> struct SingleChain
{
	SingleChain<T>* next;
	T node;
};

template<typename T> void appendSingleChain(SingleChain<T>* in_pNewHeadSingleChain,
											SingleChain<T>** in_ppFutureTailSingleChain)
{
	assert(in_pNewHeadSingleChain->next==NULL);

	in_pNewHeadSingleChain->next=*in_ppFutureTailSingleChain;
	*in_ppFutureTailSingleChain = in_pNewHeadSingleChain;
}

#endif
