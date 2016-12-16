#pragma once
#include "defines.h"
#include "sprite.h"
#include "Object.h"

#define MAXPROJ 3

class CProjectile : public CGameObject {
	friend class CGameObject;

  Vector3 Home;

	public:
		int m_nStuck;
		BOOL m_bIsStuck;
		BOOL m_bInAir;
		CProjectile() : CGameObject(PROJECTILE, Vector3(0, 0, 0), Vector3(0, 0, 0), nullptr) {};
		CProjectile(Vector3 s) : CGameObject(PROJECTILE, s, Vector3(0, 0, 0), nullptr) {};
		CProjectile(Vector3 s, Vector3 v) : CGameObject(PROJECTILE, s, v, nullptr) {};
		CProjectile(Vector3 s, Vector3 v, C3DSprite *sprite) : CGameObject(PROJECTILE, s, v, sprite) {
			Home = s; m_bIsStuck = FALSE; m_bInAir = FALSE; m_nStuck = 2000;
		}; //constructors
    void Throw(Vector3 Pos, Vector3 Vel);
    void Reset();
};