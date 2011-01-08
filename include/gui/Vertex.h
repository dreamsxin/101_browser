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

#ifndef _Vertex_h
#define _Vertex_h

#include <cmath>

template <typename Type> struct Vector2
{
	Type x, y;

	Vector2() { }
	Vector2(Type in_x, Type in_y) : x(in_x), y(in_y) { }

	Vector2<Type> operator+(Vector2<Type> in_v)
	{
		return Vector2<Type>(x+in_v.x, y+in_v.y);
	}

	Vector2<Type> operator*(Type t)
	{
		return Vector2<Type>(x*t, y*t);
	}
};

inline void normalize(Vector2<float>* in_pV)
{
	float length = sqrtf(in_pV->x*in_pV->x+in_pV->y*in_pV->y);
	in_pV->x/=length;
	in_pV->y/=length;
}

template <typename Type> Vector2<Type> normal(Vector2<Type> in_pV)
{
	return Vector2<Type>(in_pV.y, -in_pV.x);
}

/*!
 * A matrix stored in column order (as requested by OpenGL by default)
 */
template <typename Type> struct Matrix22
{
	Type x11, x21, x12, x22;

	Matrix22() { }
	Matrix22(Type in_x11, Type in_x21, Type in_x12, Type in_x22) 
		: x11(in_x11), x21(in_x21), x12(in_x12), x22(in_x22) { }

	inline Vector2<Type> operator*(Vector2<Type> in_v)
	{
		return Vector2<Type>(x11*in_v.x+x12*in_v.y, x21*in_v.x+x22*in_v.y);
	}

	inline Matrix22<Type> transpose()
	{
		return Matrix22<Type>(x11, x12, x21, x22);
	}
};

template <typename Type> struct Vertex2
{
	Type x, y;

	Vertex2() { }
	Vertex2(Type in_x, Type in_y) : x(in_x), y(in_y) { }

	Vector2<Type> operator-(Vertex2<Type> in_v)
	{
		return Vector2<Type>(x-in_v.x, y-in_v.y);
	}

	Vertex2<Type> operator+(Vector2<Type> in_v)
	{
		return Vertex2<Type>(x+in_v.x, y+in_v.y);
	}
};



#endif
