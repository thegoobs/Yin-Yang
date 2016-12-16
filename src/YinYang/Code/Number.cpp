#include "number.h"
#include "Object.h"
#include "ImageFileNameList.h"

extern CImageFileNameList g_cImageFileName;
extern int g_nScreenHeight;
extern int g_nScreenWidth;
extern int g_nScore;

//constructor
CFont::CFont() {
	for (int i = 0; i < 10; i++) {
		m_vSprites.push_back(new C3DSprite); //initialize sprites
	}
/*
	// shader initialization
	m_pShader = new CShader(2, HUD);

	m_pShader->AddInputElementDesc(0, DXGI_FORMAT_R32G32B32_FLOAT, "POSITION");
	m_pShader->AddInputElementDesc(12, DXGI_FORMAT_R32G32_FLOAT, "TEXCOORD");
	m_pShader->VSCreateAndCompile(L"VertexShader.hlsl", "main");
	m_pShader->PSCreateAndCompile(L"Hud.hlsl", "main", HUD);

	m_pShader->SetShaders(HUD);*/
}

void CFont::load() {
	for (int i = 0; i < 10; i++) {
		m_vSprites[i]->Load(g_cImageFileName[23 + i]);
	}//for
}

//index is index of vector for font
//x is x coor
//y is y coor
void CFont::draw(int index, int x, int y) {
	m_vSprites[index]->Draw(Vector3(x, y, 250.0f));
}

void CFont::drawScore(int score) {
	int offset = 0, xPos = 175;
	vector<int> s;
	if (score == 0) {
		m_vSprites[0]->Draw(Vector3(xPos - 48, g_nScreenHeight - 48, 249.0f));
		return;
	}//if

	while (score > 0) {
		int index = score % 10;
		s.push_back(index);
		score /= 10;
	}//while

	xPos += 24 * (s.size() - 3);
	for (int i = 0; i < s.size(); i++) {
		m_vSprites[s[i]]->Draw(Vector3(xPos - (24 * i), g_nScreenHeight - 48, 249.0f));
	}//for
}

void CFont::drawWave(int wave) {
	int offset = 0, xPos = g_nScreenWidth - 32;
	while (wave > 0) {
		int index = wave % 10;
		m_vSprites[index]->Draw(Vector3(xPos - (24 * offset), g_nScreenHeight - 16, 249.0f));
		wave /= 10;
		offset++;
	}//while

	
}
