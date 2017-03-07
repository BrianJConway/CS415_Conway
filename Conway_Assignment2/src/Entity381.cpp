/*
 * Entity381.cpp
 *
 *  Created on: Mar 6, 2017
 *      Author: brianconway
 */

#include <string>
#include "Entity381.h"

using namespace std;

Entity381::Entity381()
{
  // Initialize default values
  speed = 0;
  minSpeed = 0;
  heading = 0;
  desiredSpeed = 0;
  desiredHeading = 0;
  isSelected = false;
}

void Entity381::Tick(float dt)
{
  // Initialize function/variables
  int index;

  // Loop through all aspects
  for(index = 0; index < aspects.size(); index++)
    {
      // Tick each entity
      aspects[index].Tick(dt);
    }
  //end loop
}

void Entity381::setSelected(bool selected)
{
  isSelected = selected;

  // Show bounding box if selected
  if(selected)
    {
      node->showBoundingBox(true);
    }
  else
    {
      node->showBoundingBox(false);
    }
}

Destroyer::Destroyer(Ogre::SceneManager* sceneManager, Ogre::Vector3 pos, float heading)
{
  // Increment total count of entity type
  destroyerCount++;

  // Set unique entity name and ID
  entityId = destroyerCount;
  entityName = "Destroyer";
  entityName += destroyerCount - '0';

  // Create entity
  entity = sceneManager->createEntity("ddg51.mesh");

  // Create node
  node = sceneManager->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(0,0,0));

  // Attach entity to node
  node->attachObject(entity);

  // Set position and heading
  position = pos;
  this->heading = heading;

  // Set class specific values
  turningRate = 1;
  maxSpeed = 1;
  acceleration = 1;
}

Carrier::Carrier(Ogre::SceneManager* sceneManager, Ogre::Vector3 pos, float heading)
{
  // Increment total count of entity type
  carrierCount++;

  // Set unique entity name and ID
  entityId = carrierCount;
  entityName = "Carrier";
  entityName += carrierCount - '0';

  // Create entity
  entity = sceneManager->createEntity("cvn68.mesh");

  // Create node
  node = sceneManager->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(0,0,0));

  // Attach entity to node
  node->attachObject(entity);

  // Set position and heading
  position = pos;
  this->heading = heading;

  // Set class specific values
  turningRate = 1;
  maxSpeed = 1;
  acceleration = 1;
}

Speedboat::Speedboat(Ogre::SceneManager* sceneManager, Ogre::Vector3 pos, float heading)
{
  // Increment total count of entity type
  speedboatCount++;

  // Set unique entity name and ID
  entityId = speedboatCount;
  entityName = "Speedboat";
  entityName += speedboatCount - '0';

  // Create entity
  entity = sceneManager->createEntity("cigarette.mesh");

  // Create node
  node = sceneManager->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(0,0,0));

  // Attach entity to node
  node->attachObject(entity);

  // Set position and heading
  position = pos;
  this->heading = heading;

  // Set class specific values
  turningRate = 1;
  maxSpeed = 1;
  acceleration = 1;
}

Frigate::Frigate(Ogre::SceneManager* sceneManager, Ogre::Vector3 pos, float heading)
{
  // Increment total count of entity type
  frigateCount++;

  // Set unique entity name and ID
  entityId = frigateCount;
  entityName = "Frigate";
  entityName += frigateCount - '0';

  // Create entity
  entity = sceneManager->createEntity("sleek.mesh");

  // Create node
  node = sceneManager->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(0,0,0));

  // Attach entity to node
  node->attachObject(entity);

  // Set position and heading
  position = pos;
  this->heading = heading;

  // Set class specific values
  turningRate = 1;
  maxSpeed = 1;
  acceleration = 1;
}

Alien::Alien(Ogre::SceneManager* sceneManager, Ogre::Vector3 pos, float heading)
{
  // Increment total count of entity type
  alienCount++;

  // Set unique entity name and ID
  entityId = alienCount;
  entityName = "Alien";
  entityName += alienCount - '0';

  // Create entity
  entity = sceneManager->createEntity("alienship.mesh");

  // Create node
  node = sceneManager->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(0,0,0));

  // Attach entity to node
  node->attachObject(entity);

  // Set position and heading
  position = pos;
  this->heading = heading;

  // Set class specific values
  turningRate = 1;
  maxSpeed = 1;
  acceleration = 1;
}

