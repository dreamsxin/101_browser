#ifndef _Vertex_h
#define _Vertex_h

template <typename Type> struct Vertex2
{
	Type x, y;

	Vertex2() { }
	Vertex2(Type in_x, Type in_y) : x(in_x), y(in_y) { }
};

#endif
