#include "Sphere.h"
#include "../Gizmos.h"

Sphere::Sphere(vec2 position, vec2 velocity, float mass, float radius, vec4 color) : RigidBody(position, velocity, 0, mass)
{
	this->radius = radius;
	this->color = color;
	shapeID = ShapeID::SPHERE;
}
Sphere::~Sphere() {}

void Sphere::MakeGizmo()
{
	Gizmos::add2DCircle(position, radius, 20, color);
}
