#include "PhysicsScene.h"
#include "PhysicsActor.h"

#include "../ImguiImpl.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"

#include "../Gizmos.h"
#include "../Camera.h"

#include "Sphere.h"
#include "Plane.h"
#include "box.h"

using glm::vec2;
using glm::vec3;
using glm::vec4;

// Function pointer definition
typedef bool(*CollisionFunc)(PhysicsActor*, PhysicsActor*);

// Array of all available collision check functions
static CollisionFunc collisionFunctionArray[] =
{
	PhysicsScene::Plane2Plane,  PhysicsScene::Plane2Sphere,  PhysicsScene::Plane2Box,  PhysicsScene::Plane2AABB,  PhysicsScene::Plane2Joint,
	PhysicsScene::Sphere2Plane, PhysicsScene::Sphere2Sphere, PhysicsScene::Sphere2Box, PhysicsScene::Sphere2AABB, PhysicsScene::Sphere2Joint,
	PhysicsScene::Box2Plane,    PhysicsScene::Box2Sphere,    PhysicsScene::Box2Box,    PhysicsScene::Box2AABB,    PhysicsScene::Box2Joint,
	PhysicsScene::AABB2Plane,   PhysicsScene::AABB2Sphere,   PhysicsScene::AABB2Box,   PhysicsScene::AABB2AABB,   PhysicsScene::AABB2Joint,
	PhysicsScene::Joint2Plane,  PhysicsScene::Joint2Sphere,  PhysicsScene::Joint2Box,  PhysicsScene::Joint2AABB,  PhysicsScene::Joint2Joint
};

PhysicsScene::PhysicsScene() {}
PhysicsScene::~PhysicsScene() 
{
	for (auto actorPtr : actors)
	{
		delete actorPtr;
	}
	actors.clear();
}

void PhysicsScene::Update(vec2 gravity, float deltaTime)
{
	CheckForCollisions();

	for (auto actorPtr : actors)
	{
		actorPtr->Update(gravity, deltaTime);
	}
}

void PhysicsScene::Render()
{
	for (auto actorPtr:actors)
	{
		actorPtr->MakeGizmo();
		actorPtr->Debug();
	}

	Gizmos::draw2D(glm::ortho<float>(-100, 100, -100 / (16 / 9), 100 / (16 / 9), -1.0f, 1.0f));
	ImGui::Render();
}

void PhysicsScene::AddActor(PhysicsActor* actor)
{
	if(actor != nullptr)
		actors.push_back(actor);
}

void PhysicsScene::RemoveActor(PhysicsActor* actor)
{
	std::vector<PhysicsActor*>::iterator it = actors.begin();
	for (; it != actors.end(); ++it)
	{
		if ((*it) == actor)
		{
			delete (*it);
			actors.erase(it);
		}
	}
}

void PhysicsScene::CheckForCollisions()
{
	int actorCount = actors.size();
	// Need to check for collisions against all objects except this one.
	for (int outer = 0; outer < actorCount; outer++)
	{
		for (int inner = outer + 1; inner < actorCount; inner++)
		{
			PhysicsActor* object1 = actors[outer];
			PhysicsActor* object2 = actors[inner];
			int _shapeID1 = object1->shapeID;
			int _shapeID2 = object2->shapeID;

			// Grab index into func array based on enum
			int functionIndex = (_shapeID1 * NUMBER_OF_SHAPES) + _shapeID2;

			CollisionFunc collisionFunctionPtr = collisionFunctionArray[functionIndex];

			if (collisionFunctionPtr != NULL)
			{
				collisionFunctionPtr(object1, object2);
			}
		}
	}
}

///
/// PLANE COLLISIONS CHECKS
///

bool PhysicsScene::Plane2Plane(PhysicsActor * actor1, PhysicsActor * actor2)
{
	// Cast Actors
	Plane* p1 = dynamic_cast<Plane*>(actor1);
	Plane* p2 = dynamic_cast<Plane*>(actor2);

	if (p1 != NULL && p2 != NULL)
	{
		if (p1->normal == p2->normal)
			return true;
		else if (p1->normal == -p2->normal)
			return true;
	}

	return false;
}

bool PhysicsScene::Plane2Sphere(PhysicsActor* actor1, PhysicsActor* actor2)
{
	return Sphere2Plane(actor2, actor1);
}

bool PhysicsScene::Plane2Box(PhysicsActor* actor1, PhysicsActor* actor2)
{
	return Box2Plane(actor2, actor1);
}

bool PhysicsScene::Plane2AABB(PhysicsActor* actor1, PhysicsActor* actor2)
{
	return AABB2Plane(actor2, actor1);
}

bool PhysicsScene::Plane2Joint(PhysicsActor* actor1, PhysicsActor* actor2)
{
	return Joint2Plane(actor2, actor1);
}

///
/// SPHERE COLLISIONS CHECKS
///

bool PhysicsScene::Sphere2Plane(PhysicsActor * actor1, PhysicsActor * actor2)
{
	// Cast Actors
	Sphere* s = dynamic_cast<Sphere*>(actor1);
	Plane* p = dynamic_cast<Plane*>(actor2);

	if (s != NULL && p != NULL)
	{
		vec2 colNormal = p->normal;
		float sphereToPlane = glm::dot(s->position, p->normal) - p->distFromOrig;

		if (sphereToPlane < 0)
		{
			colNormal *= -1;
			sphereToPlane *= -1;
		}

		float intersection = s->radius - sphereToPlane;

		if (intersection > 0)
		{
			vec2 pNormal = p->normal;
			if (sphereToPlane < 0)
				pNormal *= -1;

			vec2 forceVector = -1 * s->mass * pNormal * (glm::dot(pNormal, s->velocity));
			s->ApplyForce(2 * forceVector);
			s->position += colNormal * intersection * .5f;

			return true;
		}
	}

	return false;
}

bool PhysicsScene::Sphere2Sphere(PhysicsActor * actor1, PhysicsActor * actor2)
{
	Sphere* s1 = dynamic_cast<Sphere*>(actor1);
	Sphere* s2 = dynamic_cast<Sphere*>(actor2);

	if (s1 != NULL && s2 != NULL)
	{
		vec2 delta = s2->position - s1->position;
		float distance = glm::length(delta);
		float intersection = s1->radius + s2->radius - distance;
		if (intersection > 0)
		{
			vec2 colNormal = glm::normalize(delta);
			vec2 relVelocity = s1->velocity - s2->velocity;
			vec2 colVector = colNormal * (glm::dot(relVelocity, colNormal));
			vec2 forceVector = colVector * 1.0f / (1 / s1->mass + 1 / s2->mass);
			float combinedElasticity = (s1->elasticity + s2->elasticity) / 2;
			s1->ApplyForce(s2, forceVector + (forceVector * combinedElasticity));

			vec2 seperationVector = colNormal * intersection * .5f;

			if(!s1->isKinematic)
				s1->position -= seperationVector;

			if(!s2->isKinematic)
				s2->position += seperationVector;

			return true;
		}
	}

	return false;
}

bool PhysicsScene::Sphere2Box(PhysicsActor* actor1, PhysicsActor* actor2)
{
	return false; //TODO
}

bool PhysicsScene::Sphere2AABB(PhysicsActor* actor1, PhysicsActor* actor2)
{
	return false; //TODO
}

bool PhysicsScene::Sphere2Joint(PhysicsActor* actor1, PhysicsActor* actor2)
{
	return false; //TODO
}

///
/// BOX COLLISIONS CHECKS
///

bool PhysicsScene::Box2Plane(PhysicsActor* actor1, PhysicsActor* actor2)
{
	return false; //TODO
}

bool PhysicsScene::Box2Sphere(PhysicsActor* actor1, PhysicsActor* actor2)
{
	return false; //TODO
}

bool PhysicsScene::Box2Box(PhysicsActor* actor1, PhysicsActor* actor2)
{
	return false; //TODO
}

bool PhysicsScene::Box2AABB(PhysicsActor* actor1, PhysicsActor* actor2)
{
	return false; //TODO
}

bool PhysicsScene::Box2Joint(PhysicsActor* actor1, PhysicsActor* actor2)
{
	return false; //TODO
}

///
/// AABB COLLISIONS CHECKS
///

bool PhysicsScene::AABB2Plane(PhysicsActor* actor1, PhysicsActor* actor2)
{
	return false; //TODO
}

bool PhysicsScene::AABB2Sphere(PhysicsActor* actor1, PhysicsActor* actor2)
{
	return false; //TODO
}
bool PhysicsScene::AABB2Box(PhysicsActor* actor1, PhysicsActor* actor2)
{
	return false; //TODO
}

bool PhysicsScene::AABB2AABB(PhysicsActor* actor1, PhysicsActor* actor2)
{
	return false; //TODO
}

bool PhysicsScene::AABB2Joint(PhysicsActor* actor1, PhysicsActor* actor2)
{
	return false; //TODO
}

///
/// JOINT COLLISIONS CHECKS
///

bool PhysicsScene::Joint2Plane(PhysicsActor* actor1, PhysicsActor* actor2)
{
	return false; //TODO
}
bool PhysicsScene::Joint2Sphere(PhysicsActor* actor1, PhysicsActor* actor2)
{
	return false; //TODO
}

bool PhysicsScene::Joint2Box(PhysicsActor* actor1, PhysicsActor* actor2)
{
	return false; //TODO
}

bool PhysicsScene::Joint2AABB(PhysicsActor* actor1, PhysicsActor* actor2)
{
	return false; //TODO
}

bool PhysicsScene::Joint2Joint(PhysicsActor* actor1, PhysicsActor* actor2)
{
	return false; //TODO
}
