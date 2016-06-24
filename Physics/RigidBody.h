#pragma once
#include "PhysicsActor.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"

using glm::vec2;
using glm::mat4;

class RigidBody : public PhysicsActor
{
public:
	RigidBody();
	RigidBody(vec2 pos, vec2 vel = vec2(0,0), float rotation = 0, float mass = 1, float elasticity = 0.6f, float linearDrag = 1.0f, float angularDrag = 1.0f);
	~RigidBody();

	const float MIN_LINEAR_THRESHOLD = 0.1f;
	const float MIN_ROTATION_THRESHOLD = 0.1f;

	vec2 position;
	vec2 velocity;
	float angularVelocity = 0;

	float rotation = 0;
	float mass = 1;
	float linearDrag = 1;
	float angularDrag = 1;
	float elasticity = 0.6f;

	bool isKinematic = false;

	mat4 rotationMatrix;

	void ApplyForce(vec2 force);
	void ApplyForce(RigidBody* otherActor, vec2 force);
	void ResetVelocity();

	// Physics Actor
	virtual void Update(vec2 gravity, float deltaTime);
	virtual void Debug() {};
	virtual void MakeGizmo() = 0;
};

