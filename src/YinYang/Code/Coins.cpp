#include "Coins.h"
#include "Timer.h"
#include "ImageFileNameList.h"
#include "Random.h"
#include "Sound.h"

extern CImageFileNameList g_cImageFileName;
extern CSoundManager* g_pSoundManager;
extern int g_nScreenWidth;
extern int g_nScreenHeight;
extern CRandom g_cRandom;
extern BOOL g_bCoinGrid[][64];
extern BOOL g_bPlatform[][64];
extern char g_nBGColors[][64];
extern CTimer g_cTimer;
extern BOOL g_bInversion;
extern int g_nScore;

void CCoinManager::checkCollision(Vector3 pos) {
	int xCor = pos.x / 16;
	int yCor = pos.y / 16;

		for (int i = 0; i < m_nNumberOfCoins; i++) {
			Vector2 temp = pos - m_vCoins[i]->m_vPos;
			if (temp.Length() < 16.0f) {
				m_vCoins[i]->m_bDestroyed = TRUE;
				g_nScore += 10;
				g_pSoundManager->play(6);
			}
		}//for
	/*
	for (auto i = m_vCoins.begin(); i != m_vCoins.end(); i++) {
		CCoin* temp = *i;
		if ((abs)(temp->m_vPos.x - pos.x) < 8 && (abs)(temp->m_vPos.y - pos.y) < 8) {
			temp->m_bDestroyed = TRUE;
			g_nScore += 10;
			return;
		}//if
	}//for
	*/
	for (auto i = m_vCoins.begin(); i != m_vCoins.end(); i++) {
		CCoin* temp = *i;
		if (temp->m_bDestroyed) {
			delete temp;
			m_vCoins.erase(i);
			m_nNumberOfCoins--; //THIS WAS THE PROBLEM FUUUUCK
			return;
		}//if
	}//for
}

void CCoinManager::init(int num) {
	m_nCurrentWave++;
	m_nNumberOfCoins = num;
	if (m_nCurrentWave == 1) { //do this once
		C3DSprite *temp = new C3DSprite;
		temp->Load(g_cImageFileName[33]); //33 = coin sprite
		m_vSprite = temp;
	}//if
	for (int i = 0; i < num; i++) {
		m_vCoins.push_back(new CCoin(Vector2(0, 0), m_vSprite));
		//initialize coins
	}//for

	//Initialize Coin Grid
	for (int i = 0; i < 48; i++) {
		for (int j = 0; j < 64; j++) {
			g_bCoinGrid[i][j] = FALSE;
		}
	}
	//mess with the coins array
	int coinCtr = 0;
	float y = g_nScreenHeight + 32;
	for (int i = 0; i < 48 && coinCtr < m_nNumberOfCoins; i++) {
		float x = 8;
		for (int j = 0; j < 64 && coinCtr < m_nNumberOfCoins; j++) {
			if (g_bPlatform[i][j] && !g_bInversion) {
				if (g_nBGColors[i][j] == 'W' && g_bCoinGrid[i][j + 1] == FALSE &&
					g_bCoinGrid[i][j - 1] == FALSE &&
					g_bPlatform[i - 1][j] == FALSE && g_cRandom.number(0, 100) > 80) {
					g_bCoinGrid[i][j] = TRUE;
					m_vCoins[coinCtr]->m_vPos = Vector2(x, y);
					coinCtr++;
				}//if
			}//if
			else if (g_bPlatform[i][j] && g_bInversion) {
				if (g_nBGColors[i][j] == 'B' && g_bCoinGrid[i][j + 1] == FALSE &&
					g_bCoinGrid[i][j - 1] == FALSE &&
					g_bPlatform[i - 1][j] == FALSE && g_cRandom.number(0, 100) > 80) {
					g_bCoinGrid[i][j] = TRUE;
					m_vCoins[coinCtr]->m_vPos = Vector2(x, y);
					coinCtr++;
				}//if
			}//else if
			x += 16;
		}//for
		y -= 16;
	}//for
}

void CCoinManager::load() {
	int coinCtr = 0;
	for (int i = 0; i < m_nNumberOfCoins; i++) {
		m_vCoins[i]->m_vPos.y +=  0.0025 * sin(0.005f * g_cTimer.time() + i);
		m_vCoins[i]->m_vSprite->Draw(Vector3(m_vCoins[i]->m_vPos.x, m_vCoins[i]->m_vPos.y, 799.0f));
	}//for
}