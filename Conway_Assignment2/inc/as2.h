/*
 * As1.h
 *
 *  Created on: Jan 24, 2017
 *      Author: sushil
 */

#ifndef __As1_h_
#define __As1_h_

#include "BaseApplication.h"
#include "EntityMgr.h"

class As2 : public BaseApplication
{
public:
  As2();
  virtual ~As2();
  virtual bool frameRenderingQueued(const Ogre::FrameEvent& fe);

  void MakeGround();
  void MakeSky();
  void MakeFog();
  void UpdateCamera(const Ogre::FrameEvent& fe);
  void UpdateSelection();
  //------------------------------------------------------------

  EntityMgr* entityMgr;
  Ogre::SceneNode* cameraNode;
  float keyboardTimer;
  const static float keyTime = 0.01f;

protected:
  virtual void createScene();
  //virtual void createCamera();
  //virtual void createViewports();
};

#endif // #ifndef __As1_h_
