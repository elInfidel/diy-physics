#pragma once
#include "PhysicsActor.h"

class Plane : public PhysicsActor
{
public:
	Plane(vec2 normal, float distanceFromOrigin);
	~Plane();

	float distFromOrig;
	vec2 normal = normal;

	virtual void Update(vec2 gravity, float deltaTime) {}
	virtual void Debug() {}
	virtual void MakeGizmo();
};

