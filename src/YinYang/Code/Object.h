/// \file object.h
/// \brief Interface for the game object class CGameObject.

#pragma once

#include "sprite.h"
#include "defines.h"
#include "Timer.h"
#include <vector>

/// \brief The game object. 
///
/// Game objects are responsible for remembering information about 
/// themselves - including their image, location, and speed - and
/// for moving and drawing themselves.

class CGameObject{ //class for a game object
  friend class CObjectManager;
  friend class CGameRenderer;

  protected:
	ObjectType m_eObjectType;
    Vector3 m_vPos; ///< Current location.
    Vector3 m_vVel; ///< Current velocity.
    int m_nLastMoveTime; ///< Last time moved.
	BOOL m_bOrientation; //left = true, right = false;
	BOOL m_bDetected;
	BOOL m_bDestroyed;

	//hit timer stuff
	CTimer m_vHitTimer; //timer to check if ninja has been hit in a while
	int m_nLastHitTime;
	int m_nHealth;
	BOOL m_vFlicker; //test whether to make the sprite flicker or not

    C3DSprite *m_pSprite; ///< Pointer to sprite.

  public:
    CGameObject(ObjectType objType, const Vector3& s, const Vector3& v, C3DSprite *sprite); ///< Constructor.
	void detection();
    void draw(); ///< Draw at current location.
    void move(); ///< Change location depending on time and speed
	void jump(); //Addition for Lab 4: jump function prototype
	void dropDown(Vector3 t); //function
	void shoot(Vector3); //shoot a ninja star
	void setVel(Vector3 v) { m_vVel = v; };
	void setPos(Vector3 p) { m_vPos = p; };
	int getHealth() { return m_nHealth; };
	BOOL getDetected() { return m_bDetected; }; //for AI
	Vector3 getPos() { return m_vPos; }; //give position
	Vector3 getVel() { return m_vVel; }; //give velocity
	void changeHealth(int i) { m_nHealth += i; return; };
	void setHealth(int i) { m_nHealth = i; return; };
	void changeSprite();
	void hitDetection(CGameObject *);
}; //CGameObject

