/*
 * Entity381.h
 *
 *  Created on: Mar 6, 2017
 *      Author: brianconway
 */

#ifndef INC_ENTITY381_H_
#define INC_ENTITY381_H_

#include <string>

#include "EntityMgr.h"

#include <cstdlib>
#include <vector>

#include "Aspect.h";

using namespace std;

class Entity381
{
public:
  Entity381();
  void Tick(float dt);
  void setSelected(bool selected);

  int entityId;
  string entityName, meshfileName;
  float minSpeed, maxSpeed, speed, heading,
	desiredSpeed, desiredHeading, turningRate, acceleration;
  bool isSelected;
  Ogre::Vector3 position, velocity;
  Ogre::SceneNode* node;
  Ogre::Entity* entity;

  vector<Aspect> aspects;
private:

};

class Destroyer : public Entity381
{
public:
  Destroyer(Ogre::SceneManager* sceneManager, Ogre::Vector3 pos, float heading);

private:
  static int destroyerCount = 0;

};

class Carrier : public Entity381
{
public:
  Carrier(Ogre::SceneManager* sceneManager, Ogre::Vector3 pos, float heading);

private:
  static int carrierCount = 0;

};

class Speedboat : public Entity381
{
public:
  Speedboat(Ogre::SceneManager* sceneManager, Ogre::Vector3 pos, float heading);

private:
  static int speedboatCount = 0;

};

class Frigate : public Entity381
{
public:
  Frigate(Ogre::SceneManager* sceneManager, Ogre::Vector3 pos, float heading);

private:
  static int frigateCount = 0;

};

class Alien : public Entity381
{
public:
  Alien(Ogre::SceneManager* sceneManager, Ogre::Vector3 pos, float heading);

private:
  static int alienCount = 0;

};


#endif /* INC_ENTITY381_H_ */
