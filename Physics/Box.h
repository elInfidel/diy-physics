#pragma once
#include "glm/glm.hpp"
#include "RigidBody.h"

using glm::vec2;
using glm::vec4;

class Box : RigidBody
{
public:
	Box(vec2 position, vec2 velocity, float mass, float width, float height, vec4 color);
	~Box();

	float width, height;

	virtual void MakeGizmo();
};

