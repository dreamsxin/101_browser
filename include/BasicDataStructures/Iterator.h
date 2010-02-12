#ifndef _Iterator_h
#define _Iterator_h

template <typename Type, typename IteratorState> struct Iterator
{
	/*!
	 * Returns 
	 * true if the iterator is at the end
	 * false otherwise
	 */
	bool (*mpfEnd)(const IteratorState*);
	
	/*
	 * Gets a pointer to the value. If the iterator is invalid 
	 * the result is not defined.
	 */
	Type* (*mpfGet)(const IteratorState*);

	/*!
	 * Iterates the state to the next. If the iterator is
	 * at the end the result is not defined.
	 */
	void (*mpfNext)(IteratorState*);
};

#endif
