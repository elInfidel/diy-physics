#include "Box.h"
#include "../Gizmos.h"


Box::Box(vec2 position, vec2 velocity, float mass, float width, float height, vec4 color)
{
	this->position = position;
	this->velocity = velocity;
	this->mass = mass;
	this->width = width;
	this->height = height;
	this->color = color;
	shapeID = ShapeID::BOX;
}

Box::~Box()
{
}

void Box::MakeGizmo()
{
	Gizmos::add2DAABBFilled(position, vec2(width, height), color);
}
