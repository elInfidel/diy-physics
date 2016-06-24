#include "RigidBody.h"
#include "imgui/Imgui.h"
#include <iostream>
RigidBody::RigidBody() {}
RigidBody::RigidBody(vec2 pos, vec2 vel, float rotation, float mass, float elasticity, float linearDrag, float angularDrag)
	: position(pos), velocity(vel), rotation(rotation), mass(mass), linearDrag(linearDrag), angularDrag(angularDrag) {}

RigidBody::~RigidBody() {}

void RigidBody::ApplyForce(vec2 force)
{
	if (!isKinematic)
	{
		velocity += force / mass;
		velocity *= linearDrag;

		rotationMatrix = glm::rotate(rotation, glm::vec3(0.0f, 0.0f, 1.0f));
		angularVelocity *= angularDrag;

		if (glm::length(velocity) < MIN_LINEAR_THRESHOLD)
			velocity = vec2(0, 0);

		if (abs(angularVelocity) < MIN_ROTATION_THRESHOLD)
			angularVelocity = 0;
	}
}

void RigidBody::ApplyForce(RigidBody* otherActor, vec2 force)
{
	otherActor->ApplyForce(force);
	ApplyForce(-force);
}

void RigidBody::ResetVelocity()
{
	velocity = vec2(0,0);
}

void RigidBody::Update(vec2 gravity, float deltaTime)
{
	ApplyForce(gravity * mass * deltaTime);

	position += velocity * deltaTime;
	rotation += angularVelocity * deltaTime;
}
