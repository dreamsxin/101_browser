#include "GuiOpenGL/GuiComponentsBasic.h"

#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include <cassert>

void createStraightBorder(Vertex2<float> prevVertex, 
						  Vertex2<float> currVertex,
						  Vertex2<float> nextVertex,
						  std::vector<Vertex2<float> >* pBorderTriangleStrip,
						  float borderWidth, 
						  size_t)
{
	Vector2<float> prevToCurrVect = currVertex - prevVertex;
	Vector2<float> currToNextVect = nextVertex - currVertex;

	normalize(&prevToCurrVect);
	normalize(&currToNextVect);

	Vector2<float> direction = prevToCurrVect+currToNextVect;
	normalize(&direction);

	Vector2<float> rightFromDirection = normal(direction);

	float scaleFactor = 
		-borderWidth/(currToNextVect.x*rightFromDirection.x+
		currToNextVect.y*rightFromDirection.y);

	pBorderTriangleStrip->push_back(currVertex);
	pBorderTriangleStrip->push_back(currVertex+rightFromDirection*scaleFactor);
}

void createRoundBorder(Vertex2<float> prevVertex, 
					   Vertex2<float> currVertex,
					   Vertex2<float> nextVertex,
					   std::vector<Vertex2<float> >* pBorderTriangleStrip,
					   float borderWidth, 
					   size_t curveSegmentsCount)
{
	assert(curveSegmentsCount>=1);

	Vector2<float> prevToCurrVect = currVertex - prevVertex;
	Vector2<float> currToNextVect = nextVertex - currVertex;

	normalize(&prevToCurrVect);
	normalize(&currToNextVect);

	Vector2<float> prevToCurrNormal = normal(prevToCurrVect);
	Vector2<float> currToNextNormal = normal(currToNextVect);
	
	/* 
	 * The orthogonal matrix that rotates (1, 0) to prevToCurrNormal is
	 *
	 * | prevToCurrNormal.x prevToCurrVect.x |
	 * | prevToCurrNormal.y prevToCurrVect.y |
	 *
	 * Since this is an orthogonal matrix the inverse one is this one transposed
	 */
	Matrix22<float> orth = Matrix22<float>(prevToCurrNormal.x, prevToCurrNormal.y,
		prevToCurrVect.x, prevToCurrVect.y).transpose();

	Vector2<float> angleVector = orth * currToNextNormal;
	// The order has to be y, x -- see declaration of atan2f
	float angle = atan2f(angleVector.y, angleVector.x);
	
	for (size_t i=0; i<=curveSegmentsCount; i++)
	{
		float currentAngle = i*angle/curveSegmentsCount;

		Matrix22<float> currentRotation = Matrix22<float>(
			cosf(currentAngle), sinf(currentAngle),
			-sinf(currentAngle), cosf(currentAngle));

		Vector2<float> movement = currentRotation*prevToCurrNormal*borderWidth;

		pBorderTriangleStrip->push_back(currVertex);
		pBorderTriangleStrip->push_back(currVertex+movement);
	}
}

/*!
* vertices output:
* [0]: bottom left
* [1]: bottom right
* [2]: top left
* [3]: top right
*/
void createBoxVertices(std::vector<Vertex2<float> >* boxVertices,
					   float left, float top, float width, float height,
					   float currentHeight)
{
	/*
	* 2-3
	* |\|
	* 0-1
	*/
	boxVertices->push_back(Vertex2<float>(left, currentHeight-top-height)); // bottom left
	boxVertices->push_back(Vertex2<float>(left+width, currentHeight-top-height)); // bottom right
	boxVertices->push_back(Vertex2<float>(left, currentHeight-top)); // top left
	boxVertices->push_back(Vertex2<float>(left+width, currentHeight-top)); // top right
}

void drawVertexArray(const std::vector<Vertex2<float> >* vertices, Color4<float> colors[4])
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);	

	glVertexPointer(2, GL_FLOAT, 0, &vertices->at(0));
	glColorPointer(4, GL_FLOAT, 0, colors);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void drawVertexArray(const std::vector<Vertex2<float> >* vertices, Color4<float> color)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glColor4fv(&color.r);

	glVertexPointer(2, GL_FLOAT, 0, &vertices->at(0));
	glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei) vertices->size());

	glDisableClientState(GL_VERTEX_ARRAY);
}
