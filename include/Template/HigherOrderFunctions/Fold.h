#ifndef _Fold_h
#define _Fold_h

#include <limits>
#include <cassert>

namespace OZO
{
	namespace HigherOrderFunctions
	{
		template <typename State, typename Type> State foldl(
			State (*in_foldFunction)(State, Type), State in_state, 
			Type *in_buffer, size_t in_bufferSize)
		{
			assert(in_bufferSize < std::numeric_limits<size_t>::max());

			for (size_t bufferIndex=0; bufferIndex<in_bufferSize; bufferIndex++)
			{
				in_state = (*in_foldFunction)(in_state, in_buffer[bufferIndex]);
			}

			return in_state;
		}
	}
}

#endif
