#pragma once

#include "Sprite.h"
#include <vector>


class CFont {
	friend class CObject;
	friend class CGameRenderer;

	protected:
		vector<C3DSprite*> m_vSprites; //collection of sprites for the numbers
		CShader* m_pShader; // shader for the text

	public:
		CFont();
		void load(); //load the font
		void draw(int, int, int); //given an index, draw the sprite
		void drawScore(int); //draw the score given the score
		void drawWave(int); //draw the wave given the wave
};
