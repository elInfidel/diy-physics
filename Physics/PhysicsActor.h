#pragma once
#include "glm/glm.hpp"

using glm::vec2;
using glm::vec4;

enum ShapeID : int
{
	PLANE = 0,
	SPHERE,
	BOX,
	AABB,
	JOINT,
	NUMBER_OF_SHAPES
};

class PhysicsActor
{
public:
	PhysicsActor();
	~PhysicsActor();

	ShapeID shapeID;
	vec4 color;

	static int actorCount;

	virtual void Update(vec2 gravity, float deltaTime) = 0;
	virtual void Debug() = 0;
	virtual void MakeGizmo() = 0;
	void virtual resetPosition() {};
};

