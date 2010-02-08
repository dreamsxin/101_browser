#ifndef _Iterator_h
#define _Iterator_h

template <typename State, typename Type> struct Iterator
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
	bool (*mfpEnd)();
	
	/*
	 * Gets the value. If the iterator is invalid 
	 * the result is not defined.
	 */
	Type* (*mfpGet)(const State* const);

	/*!
	 * Iterates the state to the next. If the iterator is
	 * at the end the result is not defined.
	 */
	void (*mfpNext)(State*);
};

#endif
