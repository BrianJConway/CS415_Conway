/*
 * EntityMgr.cpp
 *
 *  Created on: Mar 6, 2017
 *      Author: brianconway
 */

#include "EntityMgr.h";

EntityMgr::EntityMgr(Ogre::SceneManager* mSceneManager)
{
  sceneManager = mSceneManager;
  selectedEntity = -1;
}

Entity381* EntityMgr::CreateEntity(EntityType type, Ogre::Vector3 pos, float heading)
{
  // Initialize function/variables
  Entity381* newEntity;

  // Check type of entity to create
  switch(type)
  {
    case Destroyer:
      // Create Entity381
      newEntity = new Destroyer(sceneManager,pos,heading);
      break;

    case Carrier:
      newEntity = new Carrier(pos,heading);
      break;

    case Speedboat:
      newEntity = new Speedboat(pos,heading);
      break;

    case Frigate:
      newEntity = new Frigate(pos,heading);
      break;

    default:
      newEntity = new Alien(pos,heading);
      break;
  }

  // Add entity to vector
  entities.push_back(*newEntity);

  // Deselect all other entities
  deselectAll();

  // Select newly created entity
  newEntity->setSelected(true);
  selectedEntity = entities.size() - 1;

  // Return pointer to created entity
  return newEntity;
}

void EntityMgr::Tick(float dt)
{
  // initialize function/variables
  int index;

  // Loop through all managed entities
  for(index = 0; index < entities.size(); index++)
    {
      // Tick each entity
      entities[index].Tick(dt);
    }
  //end loop
}

void EntityMgr::deselectAll()
{
  // initialize function/variables
  int index;

  // Loop through all managed entities
  for(index = 0; index < entities.size(); index++)
    {
      // Tick each entity
      entities[index].isSelected = false;
    }
  //end loop
}

void EntityMgr::selectNextEntity()
{
  // Check if entities exist
  if(entities.size() > 0)
    {
      // Increment selected entity
      selectedEntity++;

      // Check if passed last element
      if( selectedEntity == entities.size() )
	{
	  selectedEntity = 0;
	}
    }

}

