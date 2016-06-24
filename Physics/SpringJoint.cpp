#include "SpringJoint.h"
#include "glm/glm.hpp"
#include "../Gizmos.h"

SpringJoint::SpringJoint(RigidBody * connection1, RigidBody * connection2, float springCoefficient, float damping, glm::vec4 color)
{
	connections[0] = connection1;
	connections[1] = connection2;

	this->color = color;
	this->springCoefficient = springCoefficient;
	this->damping = damping;
	this->restLength = glm::distance(connections[1]->position, connections[0]->position);
	shapeID = ShapeID::JOINT;
}

SpringJoint::~SpringJoint()
{

}

vec2 SpringJoint::CalculateHookesLaw(vec2 dirToPoint, vec2 velocity)
{
	// F = -k(|x|-d)(x/|x|) - bv
	float length = glm::length(dirToPoint);
	vec2 force = -springCoefficient * (length - restLength) * (glm::normalize(dirToPoint)) - (damping * velocity);

	return force;
}

void SpringJoint::Update(vec2 gravity, float deltaTime)
{
	float dist = glm::distance(connections[0]->position, connections[1]->position);

	//TODO Implement optimization for kinematic actors to avoid hookes calculation
	connections[0]->ApplyForce(CalculateHookesLaw(connections[0]->position - connections[1]->position,
		connections[0]->velocity + connections[1]->velocity) * deltaTime);

	connections[1]->ApplyForce(CalculateHookesLaw(connections[1]->position - connections[0]->position,
		connections[1]->velocity + connections[1]->velocity) * deltaTime);
}

void SpringJoint::Debug()
{
}

void SpringJoint::MakeGizmo()
{
	Gizmos::add2DLine(connections[0]->position, connections[1]->position, color);
}
