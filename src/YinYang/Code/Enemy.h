#pragma once

#include "sprite.h"
#include "defines.h"
#include "Object.h"
#include "Timer.h"
#include "Ai.h"

// Basic enemy classes
/* I need to go back and take a look at inheritance.
   The idea is to have a base class, and then two children classes with the
   respective land and air functions. ?
   CEnemy is the enemy class, while CEnemyA and CEnemyL are the air and land
   enemies, respectively.
*/

enum enemyState {
	PATROL, RETURN, ALERT, ALERT_CONTINUE, DESTROYED
};

extern C3DSprite* g_pEnemyASprite; //enemy sprite pointer
class CEnemyObject : public CIntelligentObject
{

	protected:
		Vector3 m_vLastPatrolPos; //position that enemy left to go towards ninja
		Vector3 m_vVelocity; // movement speed
		BOOL m_vDestroyed; //TRUE == destroyed
		BOOL m_bCircle; //TRUE for clockwise
		int m_nLastAiTime;
		int m_nAiDelayTime;
		//placeholder m_vOrientation;
		/* There's no data type called placeholder,
			I just don't know what data type I want this to be yet.	*/
		int m_vHealth; // self explanatory
		enemyState m_eState;
		C3DSprite *m_pSprite; // sprite pointer yippee
		C3DSprite *m_pDetectSprite;

		//timer stuff
		CTimer m_vThinkTimer; //timer to check for thinking
		CTimer m_vShootTimer; //timer to check for elapsed shoot time
		int m_nLastThinkTime = 0;
		int m_nLastShootTime = 0;

		void think(); //ai overload
		void PatrolAi(); //what to do on patrol ai
		void AlertAi(); //what to do on alert ai
		void ReturnAi();

	public:
		/* int resources will determine both health and velocity. */
		CEnemyObject(const Vector3& spawn_point, C3DSprite *sprite, int h, ObjectType t) : CIntelligentObject(t, spawn_point, Vector3(-2, 0, 0), sprite),
			m_pSprite(sprite), m_vHealth(h), m_vLastPatrolPos(spawn_point), m_vDestroyed(FALSE) {
			m_vThinkTimer.start();
			m_pDetectSprite = nullptr;
			m_bCircle = TRUE;
		}; //constructor
		void move(); //movement NOW WITH THINK
		void setLastPatrolPos(Vector3 v) { m_vLastPatrolPos = v; return; };
		void SetState(enemyState state);
		enemyState getState() { return m_eState; };
		BOOL checkDestruction();
};