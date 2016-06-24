#include "Plane.h"
#include "../Gizmos.h"


Plane::Plane(vec2 normal, float distanceFromOrigin)
{
	this->normal = normal;
	this->distFromOrig = distanceFromOrigin;
	shapeID = ShapeID::PLANE;
}

Plane::~Plane()
{
}

void Plane::MakeGizmo()
{
	float lineSegmentLength = 300;
	glm::vec2 centrePoint = normal * distFromOrig;
	glm::vec2 parallel = glm::vec2(normal.y,	-normal.x);

	//easy to rotate normal through 90 degrees around z
	glm::vec4 colour(1, 1, 1, 1);
	glm::vec2 start = centrePoint + (parallel * lineSegmentLength);
	glm::vec2 end = centrePoint-(parallel *lineSegmentLength);

	Gizmos::add2DLine(start, end, colour);
}


