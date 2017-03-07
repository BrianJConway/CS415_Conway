/*
 * Aspect.h
 *
 *  Created on: Mar 6, 2017
 *      Author: brianconway
 */

#ifndef INC_ASPECT_H_
#define INC_ASPECT_H_

class Aspect
{
public:
  Aspect(Entity381* Entity);
  virtual void Tick(float dt) = 0;

  Entity381* entity;
private:

};

class Physics : public Aspect
{
public:
  Physics(Entity381* Entity);
  void Tick(float dt);
};

class Renderable : public Aspect
{
public:
  Renderable(Entity381* Entity);
  void Tick(float dt);
};



#endif /* INC_ASPECT_H_ */
