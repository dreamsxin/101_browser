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
