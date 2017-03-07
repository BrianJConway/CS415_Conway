/*
 * EntityMgr.h
 *
 *  Created on: Mar 6, 2017
 *      Author: brianconway
 */

#ifndef INC_ENTITYMGR_H_
#define INC_ENTITYMGR_H_

#include "Entity381.h"
#include <vector>

using namespace std;

enum EntityType
{
  Destroyer,
  Carrier,
  Speedboat,
  Frigate,
  Alien
};

class EntityMgr
{
public:
  EntityMgr(Ogre::SceneManager* mSceneManager);

  Entity381* CreateEntity(EntityType type, Ogre::Vector3 pos, float heading);
  void Tick(float dt);
  void deselectAll();
  void selectNextEntity();

private:
  vector<Entity381> entities;
  Entity381* selectedEntity;
  Ogre::SceneManager* sceneManager;
  int selectedEntity;
};


#endif /* INC_ENTITYMGR_H_ */
