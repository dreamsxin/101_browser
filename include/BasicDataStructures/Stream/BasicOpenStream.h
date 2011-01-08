/*
 * Copyright 2008-2011 Wolfgang Keller
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _BasicOpenStream_h
#define _BasicOpenStream_h

/*!
 * Implements a stream with the ability to be opened and closed
 */
template <typename OpenType> class BasicOpenStream
{
public:
	virtual ~BasicOpenStream<OpenType>() { }

	/*!
	 * Return value:
	 * true:  success
	 * false: failure (e. g. file not found or permission denied)
	 */
	virtual bool open(OpenType in_openType) = 0;
	
	/*!
	 * Closes the stream if it is open. If it is not open it does nothing
	 *
	 * It should also be called in the destructor (on the reason it is a pure
	 * virtual function here we can't use it in BasicStream<OpenType>'s 
	 * destructor)
	 */
	virtual void close() = 0;

	/*!
	 * At the beginning this must return false
	 * After open(...) is called it must return true
	 * After close() is called it must return false
	 */
	virtual bool isOpen() const = 0;
};

#endif
