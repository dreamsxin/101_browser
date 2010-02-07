#ifndef _Maybe_h
#define _Maybe_h

template <typename T> struct Maybe
{
	bool valid;
	T data; // is only defined if valid == true

	inline Maybe<T>() : valid(false) { }

	inline Maybe<T>(T in_data) : valid(true), data(in_data) { }
};

#endif
