#ifndef _Vertex_h
#define _Vertex_h

struct Vertex
{
	float x, y, z;

	Vertex(float in_x, float in_y) : x(in_x), y(in_y), z(0.0f) { }

	Vertex(float in_x, float in_y, float in_z) : x(in_x), y(in_y), z(in_z) { }
};

#endif
