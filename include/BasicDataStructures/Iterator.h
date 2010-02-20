#ifndef _Iterator_h
#define _Iterator_h

enum IterateResult
{
	/*!
	 * Is returned when the current iterator state is valid
	 * (i. e. mpfGet will not return NULL) and the next
	 * state is valid, too.
	 */
	IterateResultOK, 
	/*!
	 * is returned when the current iterator state is valid
	 * but iterating brought you back to the start (which is also
	 * valid - of course, but you probably don't want to continue
	 * iterating)
	 */
	IterateResultEndToStart,
	/*!
	 * is returned if either the current state is invalid (then the
	 * next state has to be invalid, too!) or we get to an invalid
	 * state (i. e. at the end the iterator will not go back to the
	 * beginning).
	 * If the state is already invalid it should not change it (since
	 * when we go backward (in case of a DoubleIterator it MUST return
	 * to the valid state if such existed before).
	 */
	IterateResultEndToEnd
};

template <typename Type, typename IteratorState> struct SingleIterator
{
	/*
	 * Gets a pointer to the value. If the iterator state is invalid 
	 * the result is NULL.
	 */
	Type* (*mpfGet)(const IteratorState*);

	/*!
	 * Iterates the state to the next. Returns the IterateResult
	 * as described in IterateResult's description
	 */
	IterateResult (*mpfIterate)(IteratorState*);
};

template <typename Type, typename IteratorState> struct DoubleIterator
{
	/*!
	 * As in SingleIterator
	 */
	Type* (*mpfGet)(const IteratorState*);

	/*!
	 * Iterates forward
	 */
	IterateResult (*mpfIterateNext)(IteratorState*);

	/*!
	 * Iterates backward
	 */
	IterateResult (*mpfIteratePrev)(IteratorState*);
};

#endif
