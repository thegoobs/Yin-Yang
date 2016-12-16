#pragma once

#include "defines.h"
#include "sprite.h"
#include "Object.h"
#include "GameRenderer.h"

//enum for checking which sprite to set for the ninja
enum SpriteType {
	STANDING_LEFT, STANDING_RIGHT, DEAD, JUMP_LEFT, JUMP_RIGHT, RUN_LEFT1, RUN_LEFT2, RUN_LEFT3, RUN_RIGHT1, RUN_RIGHT2, RUN_RIGHT3,
	NUM_SPRITES
};

class CNinja : public CGameObject {
  friend class CGameObject;
  friend class CGameRenderer;
 
  protected:
	  SpriteType m_nSpriteIndex; //holds the index for the sprite vector index
	  vector<C3DSprite*> m_vSprites; //holds a bunch of sprites my dude
	  C3DSprite* m_pHideSprite;

  public:
    CNinja(ObjectType objType, const Vector3& s, const Vector3& v, C3DSprite *sprite); ///< Constructor.
	void setPos(Vector3 v) { m_vPos = v; };
	void setVelocity(); //grabs keyboard input and sets velocity accordingly

};