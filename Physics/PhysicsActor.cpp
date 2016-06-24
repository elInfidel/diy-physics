#include "PhysicsActor.h"

int PhysicsActor::actorCount = 0;

PhysicsActor::PhysicsActor() { actorCount++; }
PhysicsActor::~PhysicsActor() { actorCount--; }