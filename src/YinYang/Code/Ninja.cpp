#include "object.h"
#include "Ninja.h"

#include "imagefilenamelist.h"
#include "defines.h" 
#include "timer.h" 
#include "debug.h"
#include "Enemy.h"

extern BOOL g_bFrictionCheck;
extern int g_nHealth;
extern CTimer g_cTimer;
extern CImageFileNameList g_cImageFileName;

CNinja::CNinja(ObjectType objType, const Vector3& s, const Vector3& v, C3DSprite *sprite) : CGameObject(objType, s, v, sprite) {
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
  }//if;

   //load images into the sprites vector
  for (int i = 0; i < NUM_SPRITES; i++) {
	  m_vSprites.push_back(new C3DSprite());
	  switch (i) {
	  case 0: // ninja left
		  m_vSprites[i]->Load(g_cImageFileName[12]);
		  break;

	  case 1: // ninja right
		  m_vSprites[i]->Load(g_cImageFileName[13]);
		  break;

	  case 2: // ninja dead
		  break;

	  case 3: // jump left
		  m_vSprites[i]->Load(g_cImageFileName[9]);
		  break;

	  case 4: // jump right
		  m_vSprites[i]->Load(g_cImageFileName[10]);
		  break;

	  case 5: // left 1
		  m_vSprites[i]->Load(g_cImageFileName[35]);
		  break;

	  case 6: // left 2
		  m_vSprites[i]->Load(g_cImageFileName[36]);
		  break;

	  case 7: // left 3
		  m_vSprites[i]->Load(g_cImageFileName[37]);
		  break;

	  case 8: // right 1
		  m_vSprites[i]->Load(g_cImageFileName[38]);
		  break;

	  case 9: // right 2
		  m_vSprites[i]->Load(g_cImageFileName[39]);
		  break;

	  case 10: // right 3
		  m_vSprites[i]->Load(g_cImageFileName[40]);
		  break;
	  }
  }//for

  //load the hide image
  m_pHideSprite = new C3DSprite;
  m_pHideSprite->Load(g_cImageFileName[43]);

} //constructor

void CNinja::setVelocity() {
	SHORT keyR = GetAsyncKeyState(VK_RIGHT);
	SHORT keyL = GetAsyncKeyState(VK_LEFT);
	SHORT keyA = GetAsyncKeyState(0x41); //A key
	SHORT keyD = GetAsyncKeyState(0x44); //D key

	if ((1 << 16) & (keyR | keyD)) {
		if ((1 << 16) & keyL | keyA) m_vVel.x = 0;
		else { // moving right
			m_bOrientation = false;
			m_vVel.x = 10.0f;
			if (m_vVel.y != 0) // if in the air
			{
				m_pSprite = m_vSprites[4]; // jump sprite
			}
			else
			{
				// running right starts at index 8, ends at 10
				int t = g_cTimer.time();
				t = (t / 125) % 3;
				m_pSprite = m_vSprites[t + 8];
			}
		}
	}
	else if ((1 << 16) & (keyL | keyA)) {
		if ((1 << 16) & keyR | keyD) m_vVel.x = 0;
		else { // moving left
			m_bOrientation = true;
			m_vVel.x = -10.0f;
			if (m_vVel.y != 0) // if in the air
			{
				m_pSprite = m_vSprites[3]; // jump sprite
			}
			else
			{
				// running left starts at index 5, ends at 7
				int t = g_cTimer.time();
				t = (t / 125) % 3;
				m_pSprite = m_vSprites[t + 5];
			}
		}
	}
	else {
		if (m_bOrientation)
		{
			// if facing left
			if (m_vVel.y != 0) {
				m_pSprite = m_vSprites[3];
			}

			else m_pSprite = m_vSprites[0];
		}
		else
		{
			// if facing right
			if (m_vVel.y != 0) {
				m_pSprite = m_vSprites[4];
			}

			else m_pSprite = m_vSprites[1];
		}
		g_bFrictionCheck = TRUE;
	}
}//setVelocity