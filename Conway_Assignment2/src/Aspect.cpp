/*
 * Aspect.cpp
 *
 *  Created on: Mar 6, 2017
 *      Author: brianconway
 */

#include "Aspect.h"

Aspect::Aspect(Entity381* Entity)
{
	entity = Entity;
}

Physics::Physics(Entity381* Entity) : Aspect(Entity)
{

}

void Physics::Tick(float dt)
{
  // Adjust speed if necessary
  if(entity->speed > entity->desiredSpeed)
    {
      entity->speed -= entity->acceleration * dt;
    }
  else if(entity->speed < entity->desiredSpeed)
    {
      entity->speed += entity->acceleration * dt;
    }

  // Adjust heading if necessary
  if(entity->heading > entity->desiredHeading)
    {
      entity->speed -= entity->turningRate * dt;
    }
  else if(entity->heading < entity->desiredHeading)
    {
      entity->speed += entity->turningRate * dt;
    }

  // Calculate velocity
  entity->velocity.x = entity->speed * sin(entity->heading);
  entity->velocity.z = entity->speed * cos(entity->heading);

  // Calculate position
  entity->position += entity->velocity * dt;
}

Renderable::Renderable(Entity381* Entity) : Aspect(Entity)
{
}

void Renderable::Tick(float dt)
{
  entity->node->setPosition(entity->position);
  entity->node->yaw(Ogre::Degree(entity->heading));
}
