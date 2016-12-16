/// \file object.cpp
/// \brief Code for the game object class CGameObject.

#include "object.h"


#include "imagefilenamelist.h"
#include "defines.h" 
#include "timer.h" 
#include "debug.h"
#include "Enemy.h"
#include "Sound.h"
#include "Projectile.h"

extern CTimer g_cTimer;
extern int g_nScreenWidth;
extern int g_nScreenHeight;
extern XMLElement* g_xmlSettings;
extern BOOL g_bPlatform[][64];
extern char g_nBGColors[][64];
extern C3DSprite * g_pDetectedSprite;
extern CImageFileNameList g_cImageFileName;
extern int g_nHealth;
extern GameState g_eGameState;

extern BOOL g_bFrictionCheck;
extern CGameObject * g_pNinja;
extern C3DSprite *g_pNinjaLSprite;
extern C3DSprite *g_pNinjaRSprite;
extern C3DSprite* g_pNinjaJumpLS;
extern C3DSprite* g_pNinjaJumpRS;
extern C3DSprite* g_pNinjaRunL1;
extern C3DSprite* g_pNinjaRunL2;
extern C3DSprite* g_pNinjaRunL3;
extern C3DSprite* g_pNinjaRunR1;
extern C3DSprite* g_pNinjaRunR2;
extern C3DSprite* g_pNinjaRunR3;

//projectile object and sprite extern
extern vector<CProjectile*> g_pNinjaProjectile;
extern int g_nProjInd;
extern vector<CGameObject*> g_pProjectiles;
extern vector<CEnemyObject*> g_pEnemies;
extern C3DSprite * g_pProjectileSprite;
extern C3DSprite * g_pEnemyProjectileSprite;
extern CSoundManager *g_pSoundManager;
extern ColorScheme g_eTheme;
extern BOOL g_bInversion;
extern BOOL g_bNinjaHit;

//Addition for Lab 4: prototypes
BOOL isOnPlatformOrGround(float x, float& y);
BOOL isUnderPlatform(float x, float& y);

/// Initialize a game object. Gets object-dependent settings from g_xmlSettings
/// from the "object" tag that has the same "name" attribute as parameter name.
/// Assumes that the sprite manager has loaded the sprites already.
/// \param s Initial location of object 
/// \param v Initial velocity
/// \param sprite Pointer to the sprite for this object.

CGameObject::CGameObject(ObjectType objType, const Vector3& s, const Vector3& v, C3DSprite *sprite){ 
  m_nLastMoveTime = g_cTimer.time(); //time
  m_eObjectType = objType;
  m_vPos = s; //location
  m_vVel = v; //velocity
  m_pSprite = sprite; //sprite pointer
  m_bDestroyed = FALSE;
  setHealth(g_nHealth);
  if (m_eObjectType == NINJA) {
	  m_vHitTimer.start();
	  m_nLastHitTime = 0;
  }//if
} //constructor

void CGameObject::detection()
{
	int xCor = m_vPos.x / 16;
	int yCor = m_vPos.y / 16;
	if (g_nBGColors[48-yCor][xCor] == 'W') {

		if (g_bInversion) // if the world is inverted, switch the result of the detection
		{
			m_bDetected = FALSE; // NOT SPOTTED
		//	g_pDetectedSprite->Draw(Vector3(0, 800, 0), HUD);  //Draw out of bounds
		}
		else 
		{
			m_bDetected = TRUE; //WE'VE BEEN SPOTTED
		//	g_pDetectedSprite->Draw((Vector3(512.0f, g_nScreenHeight - 38.0f, 249.0f)), HUD);
		}
	}
	else {
		if (g_bInversion)
		{
			m_bDetected = TRUE; //WE'VE BEEN SPOTTED
		//	g_pDetectedSprite->Draw((Vector3(512.0f, g_nScreenHeight - 38.0f, 249.0f)), HUD);
		}
		else
		{
			m_bDetected = FALSE; //not detected anymore
		//	g_pDetectedSprite->Draw(Vector3(0, 800, 0), HUD);  //Draw out of bounds
		}
	}

	return;
}
/// Draw the current sprite frame at the current position, then
/// compute which frame is to be drawn next time.

void CGameObject::draw() { //draw
  if (m_eObjectType == NINJA) { //detection is only for ninja
		detection();
		if (g_bNinjaHit && m_pSprite && g_cTimer.time() % 250 >= 125) {
			m_pSprite->Draw(m_vPos, HIT);
			return;
		}
  }
  if (m_eObjectType == ENEMY_GROUND)
	  m_vPos.y -= 8.0f;
  if(m_pSprite) //if there is a sprite
    m_pSprite->Draw(m_vPos); //draw in correct place
  if (m_eObjectType == ENEMY_GROUND)
	  m_vPos.y += 8.0f;
} //draw
 
/// The distance that an object moves depends on its speed, 
/// and the amount of time since it last moved.

void CGameObject::move(){ //move object
  const float SCALE = 32.0f; //to scale back motion
  const float MARGIN = 200.0f; //margin on top of page

  const int time = g_cTimer.time(); //current time

  if (g_eGameState == PAUSE || g_eGameState == TITLE_SCREEN) m_nLastMoveTime = time;
  const int tdelta = time - m_nLastMoveTime; //time since last move
  const float tfactor = tdelta/SCALE; //scaled time factor

  m_vPos += m_vVel*tfactor; //motion

  //hit the walls
  if (m_vPos.x < 32.0f)
  {
	  if (m_eObjectType == NINJA&& m_vPos.x < 17.0f) {
		  m_vPos.x = 17.0f;
		  m_vVel.x = 0.0f;
	  }
	  else if (m_eObjectType == ENEMY_AIR || m_eObjectType == ENEMY_GROUND) {
		  m_vPos.x = 32.0f;
		  m_vVel.x = abs(m_vVel.x);
		  return;
	  }
	  else if (m_eObjectType == PROJECTILE && m_vPos.x < 8.0f) {
		  CProjectile* temp = (CProjectile*)this;
		  temp->Reset();
		  return;
	  }
    else if (m_eObjectType == PROJECTILE && m_vPos.x < 8.0f) {
      m_bDestroyed = true;
    }
  }

  if(m_vPos.x > (float)g_nScreenWidth - 32)
  {
	  if (m_eObjectType == ENEMY_AIR || m_eObjectType == ENEMY_GROUND) {
		  m_vPos.x = (float)g_nScreenWidth - 32;
		  m_vVel.x = -1.0f * abs(m_vVel.x); //change direction on wall hit
		  return; //all we need
	  }

	  if (m_eObjectType == NINJA && m_vPos.x > (float)g_nScreenWidth - 16) {
		  m_vPos.x = (float)g_nScreenWidth - 16;
		  m_vVel.x = 0.0f;
	  }

	  else if (m_eObjectType == PROJECTILE && m_vPos.x > (float)g_nScreenWidth - 8) {
		  CProjectile* temp = (CProjectile*)this;
		  temp->Reset();
	  }
  }
  
  //hit the roof
  if (m_vPos.y > (float)g_nScreenHeight - 109) { //109 = HUD + ninja height

	  if (m_eObjectType == PROJECTILE) {
		  CProjectile* temp = (CProjectile*)this;
		  if (m_vPos.y > (float)g_nScreenHeight - 77 && m_vPos != temp->Home) {
			  temp->Reset();
			  return;
		  }//if
	  }//if
	  else if (m_eObjectType == ENEMY_PROJECTILE) {
		  if (m_vPos.y > (float)g_nScreenHeight - 77)
			  m_bDestroyed = true;
	  }
	  else {
		  m_vVel.y = 0.0f;
		  m_vPos.y = (float)g_nScreenHeight - 109;
		  return;
	  }
  } //if

  if (m_vPos.y < 0 && m_eObjectType == ENEMY_PROJECTILE) {
	  m_bDestroyed = TRUE;
  }

  //falling
  if (m_eObjectType == NINJA || m_eObjectType == ENEMY_GROUND) { //only ninjas fall
	  if (isOnPlatformOrGround(m_vPos.x, m_vPos.y) && m_vVel.y <= 0.0f) {
		  if (m_eObjectType == ENEMY_GROUND && (!isOnPlatformOrGround(m_vPos.x + 16.0f, m_vPos.y) || !isOnPlatformOrGround(m_vPos.x - 16.0f, m_vPos.y)) && m_vVel.y <= 0.0f) {
			  m_vVel.x *= -1; //flip it around
		  }//if
		  m_vVel.y = 0.0f;
	  }
	  else {
		  m_vVel.y -= 4.0f*tfactor;
	  }

  }

  if (m_eObjectType == NINJA && isOnPlatformOrGround(m_vPos.x, m_vPos.y) && m_vVel.x > 0 && g_bFrictionCheck == TRUE) {//on ground & moving right
	  m_vVel.x -= 2.0f*tfactor;
  }

  else if (m_eObjectType == NINJA && isOnPlatformOrGround(m_vPos.x, m_vPos.y) && m_vVel.x < 0 && g_bFrictionCheck == TRUE) {
	  m_vVel.x += 2.0f*tfactor;
  }

  if (m_eObjectType == NINJA && abs(m_vVel.x) < 1.0f && g_bFrictionCheck == TRUE) {
	  m_vVel.x = 0;
	  g_bFrictionCheck = FALSE;
  }


  if (m_eObjectType == ENEMY_AIR) {
	  BOOL working = FALSE;
	  for (auto i = g_pEnemies.begin(); i != g_pEnemies.end(); i++) {
		  CEnemyObject *temp = *i;
		  if (temp == this) continue; //don't do to self
		  if (temp->m_vVel.y != 0.0f) continue; //bail if other enemy is going around
		  Vector3 v = getPos() - temp->getPos();
		  v.z = 0.0f;
		  if (v.Length() <= 128.0f) {
			  v.Normalize();
			  working = TRUE;
			  m_vVel.y = 2 * v.y;
		  }//if

		  else if (!working)
			  m_vVel.y = 0.0f;
	  }//for
  }//if

  /*
  if (m_eObjectType == ENEMY_GROUND) {
	  BOOL working = FALSE;
	  for (auto i = g_pEnemies.begin(); i != g_pEnemies.end(); i++) {
		  CEnemyObject *temp = *i;
		  if (temp == this || temp->m_eObjectType != ENEMY_GROUND) continue; //don't do to self
		  if (temp->m_vVel.x == 0.0f) continue; //bail if other enemy is stopping around
		  Vector3 v = getPos() - temp->getPos();
		  v.z = 0.0f;
		  if (v.Length() <= 128.0f && !working) {
			  v.Normalize();
			  working = TRUE;
			  m_vVel.x = 0.0f;
		  }//if

		  else if (working) m_vVel.x = 2 * -v.x;
	  }//for
  }//if
  */

  m_nLastMoveTime = time; //record time of move
} //move

//Addition for Lab 4: Jump function
void CGameObject::jump(){

	if (isOnPlatformOrGround(m_vPos.x, m_vPos.y)){
		m_vVel.y = 40.0f;
		m_vPos.y += 10.0f;
		g_pSoundManager->play(0);
	} //if
} //jump

//function that detects if ninja can drop throw single platforms (and not go through double platforms)
void CGameObject::dropDown(Vector3 v) {
	int xCor = m_vPos.x / 16; // get grid coordinates
	int yCor = m_vPos.y / 16;
	yCor = 47 - yCor;
	if (g_bPlatform[yCor+4][xCor] && g_bPlatform[yCor+3][xCor]) { // if double platform
		// do not do the job
		return;
		}

	// do the job!!!!
	m_vPos.x += v.x;
	m_vPos.y += v.y;
	return;
}

void CGameObject::shoot(Vector3 cursorPos)
{
	Vector3 throwDirection = cursorPos - m_vPos;
	throwDirection.Normalize();
	int pTime = g_cTimer.ProjectileTime();
	//Initialize prjectile
	if (m_eObjectType == NINJA) {
		for (int i = 0; i < MAXPROJ; i++) {
			if (!g_pNinjaProjectile[i]->m_bIsStuck && !g_pNinjaProjectile[i]->m_bInAir) {
				g_pNinjaProjectile[i]->Throw(Vector3(m_vPos.x, m_vPos.y, 850.0f - i), 40.0 * throwDirection);
				//g_nProjInd = g_nProjInd + 1;
				//g_nProjInd = g_nProjInd % MAXPROJ;
				if (throwDirection.x < 0)
					m_pSprite = g_pNinjaLSprite; //set to left sprite
				else
					m_pSprite = g_pNinjaRSprite; //set to right sprite
				if (g_eGameState == GAME)
					g_pSoundManager->play(1); //play the throw.wav sound
				return;
			}//if
		}//for
	}//if
	else {
		g_pSoundManager->play(5); //play the throw.wav sound
		g_pProjectiles.push_back(new CGameObject(ENEMY_PROJECTILE, Vector3(m_vPos.x, m_vPos.y, 400.0f), throwDirection * 20.0f, g_pEnemyProjectileSprite)); //pushback for enemy projectile
	}
}

void CGameObject::changeSprite()
{
	//moving right
	if (m_vVel.x > 0.0f) {
		m_pSprite = g_pNinjaRSprite; //set to right sprite
	}
	else if (m_vVel.x < 0.0f) { //moving left
		m_pSprite = g_pNinjaLSprite; //set to left sprite
	}
}

void CGameObject::hitDetection(CGameObject *obj)
{
  if (m_eObjectType == PROJECTILE && m_vVel.x == 0.0f && m_vVel.y == 0.0f) //if it's a projectile stuck in a wall
    return; //fuhgettaboudit
	//check the positions to see if they collide or not
	Vector2 diffPos = obj->getPos() - m_vPos; //difference in position
	if (abs(diffPos.x) <= 16.0f && abs(diffPos.y) < 32.0f) {
		//if(m_eObjectType != PROJECTILE)
		  //m_bDestroyed = TRUE;
		//check the timer if ninja has been hit in a while or not
		if (!g_bNinjaHit && g_cTimer.elapsed(obj->m_nLastHitTime, 1000)) { //has it been some time since ninja has been hit?
			obj->m_nLastHitTime = g_cTimer.time();
			obj->m_nHealth--; //lower health by one
			g_pSoundManager->play(7);
			g_bNinjaHit = TRUE;
			m_bDestroyed = TRUE;
		}//if
	}//if
}
