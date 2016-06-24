#pragma once
#include <vector>
#include <loadgen/gl_core_4_4.h>
#include "glfw/glfw3.h"
#include "glm/glm.hpp"

using glm::vec2;

class Camera;
class PhysicsActor;

class PhysicsScene
{
public:
	PhysicsScene();
	~PhysicsScene();

	vec2 gravity;

	void AddActor(PhysicsActor* actor);
	void RemoveActor(PhysicsActor* actor);

	void Update(vec2 gravity, float deltaTime);
	void Render();



	// Collision Type Checks
	static bool Plane2Plane(PhysicsActor* actor1, PhysicsActor* actor2);
	static bool Plane2Sphere(PhysicsActor* actor1, PhysicsActor* actor2);
	static bool Plane2Box(PhysicsActor* actor1, PhysicsActor* actor2);
	static bool Plane2AABB(PhysicsActor* actor1, PhysicsActor* actor2);
	static bool Plane2Joint(PhysicsActor* actor1, PhysicsActor* actor2);

	static bool Sphere2Plane(PhysicsActor* actor1, PhysicsActor* actor2);
	static bool Sphere2Sphere(PhysicsActor* actor1, PhysicsActor* actor2);
	static bool Sphere2Box(PhysicsActor* actor1, PhysicsActor* actor2);
	static bool Sphere2AABB(PhysicsActor* actor1, PhysicsActor* actor2);
	static bool Sphere2Joint(PhysicsActor* actor1, PhysicsActor* actor2);

	static bool Box2Plane(PhysicsActor* actor1, PhysicsActor* actor2);
	static bool Box2Sphere(PhysicsActor* actor1, PhysicsActor* actor2);
	static bool Box2Box(PhysicsActor* actor1, PhysicsActor* actor2);
	static bool Box2AABB(PhysicsActor* actor1, PhysicsActor* actor2);
	static bool Box2Joint(PhysicsActor* actor1, PhysicsActor* actor2);

	static bool AABB2Plane(PhysicsActor* actor1, PhysicsActor* actor2);
	static bool AABB2Sphere(PhysicsActor* actor1, PhysicsActor* actor2);
	static bool AABB2Box(PhysicsActor* actor1, PhysicsActor* actor2);
	static bool AABB2AABB(PhysicsActor* actor1, PhysicsActor* actor2);
	static bool AABB2Joint(PhysicsActor* actor1, PhysicsActor* actor2);

	static bool Joint2Plane(PhysicsActor* actor1, PhysicsActor* actor2);
	static bool Joint2Sphere(PhysicsActor* actor1, PhysicsActor* actor2);
	static bool Joint2Box(PhysicsActor* actor1, PhysicsActor* actor2);
	static bool Joint2AABB(PhysicsActor* actor1, PhysicsActor* actor2);
	static bool Joint2Joint(PhysicsActor* actor1, PhysicsActor* actor2);

private:
	void CheckForCollisions();

	// Actors in scene
	std::vector<PhysicsActor*> actors;
};

