#pragma once
#include "PhysicsActor.h"
#include "RigidBody.h"

class SpringJoint : public PhysicsActor
{
public:
	SpringJoint(RigidBody* connection1, RigidBody* connection2, float springCoefficient, float damping, glm::vec4 color = vec4(1, 1, 1, 1));
	~SpringJoint();

private:
	RigidBody* connections[2];
	float damping;
	float restLength;
	float springCoefficient;
	glm::vec4 color;

	vec2 CalculateHookesLaw(vec2 dirToPoint, vec2 velocity);

	virtual void Update(vec2 gravity, float deltaTime);
	virtual void Debug();
	virtual void MakeGizmo();
};

