#pragma once
#include "RigidBody.h"

using glm::vec4;

class Sphere : public RigidBody
{
public:
	Sphere(vec2 position, vec2 velocity, float mass, float radius, vec4 color);
	~Sphere();

	float radius;

	virtual void MakeGizmo();
};

