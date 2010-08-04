#ifndef _Fold_h
#define _Fold_h

#include <limits>
#include <cassert>

namespace MTAx
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

		template <typename State, typename Type, typename ParamType> 
		State foldlWithModificationAndParam(
			State (*in_foldFunction)(State, Type*, ParamType), State in_state, 
			Type *in_buffer, size_t in_bufferSize, ParamType param)
		{
			assert(in_bufferSize < std::numeric_limits<size_t>::max());

			for (size_t bufferIndex=0; bufferIndex<in_bufferSize; bufferIndex++)
			{
				in_state = (*in_foldFunction)(in_state, in_buffer+bufferIndex, param);
			}

			return in_state;
		}

		template <typename State, typename Type, typename ParamType> 
		State foldrWithModificationAndParam(
			State (*in_foldFunction)(Type*, State, ParamType), State in_state, 
			Type *in_buffer, size_t in_bufferSize, ParamType param)
		{
			assert(in_bufferSize < std::numeric_limits<size_t>::max());

			for (size_t bufferIndex=in_bufferSize+1; bufferIndex>0; bufferIndex--)
			{
				in_state = (*in_foldFunction)(in_buffer+bufferIndex-1, in_state, param);
			}

			return in_state;
		}

		/*
		 * Here the return value of in_foldFunction is passed in the
		 * first parameter State* and the return value of in_foldFunction
		 * tells whether to continue (true) or not (false)
		 */
		template <typename State, typename Type> State foldlWithModificationAndBreak(
			bool (*in_foldFunction)(State*, Type*), State in_state, 
			Type *in_buffer, size_t in_bufferSize)
		{
			assert(in_bufferSize < std::numeric_limits<size_t>::max());

			for (size_t bufferIndex=0; bufferIndex<in_bufferSize; bufferIndex++)
			{
				if (!(*in_foldFunction)(&in_state, in_buffer+bufferIndex))
					break;
			}

			return in_state;
		}
	}
}

#endif
