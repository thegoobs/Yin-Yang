#pragma once

#include <vector>
#include "Sprite.h"

class CCoin {
	friend class CGameObject;
	friend class CGameRenderer;
	friend class CCoinManager;

	protected:
		C3DSprite* m_vSprite;
		int m_nScoreGranted;
		BOOL m_bDestroyed;
		Vector2 m_vPos;
		int grid[2];

	public:
		CCoin(Vector2 pos, C3DSprite *sprite) : m_vPos(pos){
			m_vSprite = new C3DSprite;
			m_vSprite = sprite;
			m_bDestroyed = FALSE;
		}; //constructor
		~CCoin() {
			m_vSprite = nullptr;
			m_vPos = Vector2(0, 0);
		}
};

class CCoinManager {
	friend class CObject;
	friend class CGameRenderer;

	protected:
		C3DSprite* m_vSprite;
		int m_nNumberOfCoins; //number of coins in the vector
		int m_nCurrentWave = 0;
		vector<CCoin*> m_vCoins;
	public:
		void init(int); //reset the coins based on number of coins
		void load(); //load the coins onto the screen
		void checkCollision(Vector3); //check a collision based on a given vector
};