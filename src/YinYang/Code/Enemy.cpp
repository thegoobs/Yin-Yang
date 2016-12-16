#include "Enemy.h"

#include "defines.h"
#include "timer.h"
#include "Random.h"
#include "debug.h"
#include "sound.h"
#include "Ninja.h"
#include "Projectile.h"

extern CTimer g_cTimer;
extern CRandom g_cRandom;
extern int g_nScreenWidth;
extern int g_nScreenHeight;
extern CNinja *g_pNinja;
extern vector<CProjectile *> g_pNinjaProjectile;
extern vector<CGameObject*> g_pProjectiles;
extern CSoundManager *g_pSoundManager;
extern C3DSprite *g_pExclamationPoint;
extern int g_nProjInd;
extern GameState g_eGameState;
extern BOOL g_bPlatform[][64];
extern vector<CEnemyObject*> g_pEnemies;

void CEnemyObject::move()
{
	//think interval check (messes up the AI)
	if (((g_cRandom.number(0, 500) == 50) || m_bNinjaDetect || m_eState == RETURN) && g_eGameState != PAUSE && g_eGameState != TITLE_SCREEN) //1 / 100 chance of thought, also make sure the game isn't paused
		think();
	CGameObject::move();
}

void CEnemyObject::think() {
		CIntelligentObject::think(); //think as an intelligent object
		if (m_bNinjaDetect && m_eObjectType == ENEMY_AIR) {  //<<<SPOTTED>>>
			if (m_eState != ALERT_CONTINUE && m_eState != ALERT) { //before we set it to alert, check edge
				SetState(ALERT);
				m_vLastPatrolPos = m_vPos;
				m_pDetectSprite = g_pExclamationPoint; //set exclamation point sprite
				g_pSoundManager->play(3); //detected.wav
			}

			else SetState(ALERT_CONTINUE);
			//draw exclamation point
			Vector3 point = m_vPos;
			point.y += 44;
			m_pDetectSprite->Draw(point);
			AlertAi(); //alert
		} //if

		else if (m_eObjectType == ENEMY_AIR) {
			if (abs(m_vPos.x - m_vLastPatrolPos.x) > 0.10f && abs(m_vPos.y - m_vLastPatrolPos.y) > 0.10f) {
				SetState(RETURN);
				m_bCircle = g_cRandom.number(0, 1); //retest for clockwise alert rotation
				ReturnAi();
			}

			else if (m_eState != PATROL){
				SetState(PATROL);
				PatrolAi();
			}
		}//else if

		if (m_bNinjaDetect && m_eObjectType == ENEMY_GROUND) {  //<<<SPOTTED>>>
			if (m_eState != ALERT_CONTINUE && m_eState != ALERT) { //before we set it to alert, check edge
				SetState(ALERT);
				m_vLastPatrolPos = m_vPos;
				m_pDetectSprite = g_pExclamationPoint; //set exclamation point sprite
				g_pSoundManager->play(3); //detected.wav
			}//IF

			else SetState(ALERT_CONTINUE);
			//draw exclamation point
			Vector3 point = m_vPos;
			point.y += 44;
			m_pDetectSprite->Draw(point);
			AlertAi(); //alert
		}//if

		else if (m_eObjectType == ENEMY_GROUND) {
			if (m_eState != PATROL) {
				m_eState = PATROL;
				PatrolAi();
			}
		}//if
}

void CEnemyObject::PatrolAi()
{
	if (g_cRandom.number(0, 1) > 0.5f) //basically 50 / 50 chance
		setVel(Vector3(2, 0, 0));
	else setVel(Vector3(-2, 0, 0));
}

void CEnemyObject::AlertAi()
{
	if (m_eObjectType == ENEMY_AIR) {
		Vector3 v = g_pNinja->getPos() - getPos();
		v.z = 0;
		//math to make enemy go in circle from henceforth
		if (v.Length() < 128.0f) {
			v.Normalize(); //norm v
			float temp = v.x; //opposite reciprocals
			v.x = v.y;
			v.y = -temp;

			m_bCircle ?
				m_vVel = 2 * v : m_vVel = 2 * -v;
		}

		else {
			v.Normalize();
			setVel(v * 2.0f); //velocity towards ninja
		}

		if (m_vShootTimer.elapsed(m_nLastShootTime, g_cRandom.number(1000, 4000))) {
			this->shoot(g_pNinja->getPos());
		}
	}//if

	if (m_eObjectType == ENEMY_GROUND) {
		Vector3 prevVel = getVel();
		Vector3 v = g_pNinja->getPos() - getPos();
		v.z = 0;
		v.Normalize();
		v *= 2.0f;
		v.y = prevVel.y;
		setVel(v);

		if (m_vShootTimer.elapsed(m_nLastShootTime, g_cRandom.number(2000, 7000))) {
			this->shoot(g_pNinja->getPos());
		}//if
	}//if
}

void CEnemyObject::ReturnAi()
{
	//go back to the last position it was at before finding ninja
	Vector3 v = m_vLastPatrolPos - m_vPos;
	v.Normalize();
	setVel(v * 2.0f); //Velocity towards point
}

void CEnemyObject::SetState(enemyState state) {
	m_eState = state; //set the state
	m_nAiDelayTime = 500 + g_cRandom.number(0, 1000); //some random interval that the AI think at
	return;
}

//check if the projectile hits the enemy, and lower the enemy's health
BOOL CEnemyObject::checkDestruction()
{
	if (m_vHealth == 0) return TRUE;
  for (int c = 0; c < MAXPROJ; c++) {
    if (g_pNinjaProjectile[c] == nullptr) return FALSE; //bail if projectile doesn't exist

    Vector2 projPos = g_pNinjaProjectile[c]->getPos();
    Vector2 projVel = g_pNinjaProjectile[c]->getVel();
    if (abs(m_vPos.x - projPos.x) < 32.0f && abs(m_vPos.y - projPos.y) < 32.0f && abs(projVel.x) > 0.0f && abs(projVel.y) > 0.0f) { //32 = distance from center to edge
      m_vHealth--;
      g_pSoundManager->play(2); //play the projectile_hit_enemy.wav file
      g_pNinjaProjectile[c]->setPos(Vector3(g_nScreenWidth/2.0f, g_nScreenHeight/2.0f, 1001.0f));
	  g_pNinjaProjectile[c]->setVel(Vector3(0, 0, 0));
	  g_pNinjaProjectile[c]->m_bIsStuck = TRUE;
	  g_pNinjaProjectile[c]->m_nStuck = g_cTimer.time();
      return FALSE;
    }//if
  }//for

	return FALSE; //bail because he aint dead yet
}
