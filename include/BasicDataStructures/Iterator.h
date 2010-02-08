#ifndef _Iterator_h
#define _Iterator_h

template <typename Type, typename State> struct Iterator
{
	/*!
	 * The current state of the iterator
	 */
	State mState;

	/*!
	 * Returns 
	 * true if the iterator is at the end
	 * false otherwise
	 */
	bool (*mpfEnd)();
	
	/*
	 * Gets a pointer to the value. If the iterator is invalid 
	 * the result is not defined.
	 */
	Type* (*mpfGet)(const State* const);

	/*!
	 * Iterates the state to the next. If the iterator is
	 * at the end the result is not defined.
	 */
	void (*mpfNext)(State*);
};

#endif
