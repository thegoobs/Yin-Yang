/// \file defines.h
/// \brief Essential defines.

#pragma once

#include <d3d11_2.h>
#include <dxgi1_3.h>
#include <DirectXMath.h>

#include "SimpleMath.h"
#include "tinyxml2.h"

using namespace DirectX;
using namespace SimpleMath;
using namespace tinyxml2;
using namespace std;

//enum for different game states
enum GameState {
	TITLE_SCREEN, GAME, PAUSE, WAVE_COMPLETE, WIN, LOSE, HOW_TO_PLAY
};

//enum for color scheme
enum ColorScheme {
	HIT, YIN, YANG, LAN, CHENGZI, HONG, LUSE, HUANGSE, ZISE, NUM_SHADERS
};// NUM_SHADERS must always be the last one in the list

//enum to distinguish different objects
enum ObjectType {
	NINJA, ENEMY_GROUND, ENEMY_AIR, PROJECTILE, ENEMY_PROJECTILE, NUM_OBJECT_TYPES
};// NUM_OBJECT_TYPES needs to be the last one in the list!
/// Safe release of a pointer to a Windows COM object. If
/// the pointer is not null, then release what it is pointing
/// to and set it to null.

//Beware the extra parentheses in this #define. They are there
//for a reason. 

#define SAFE_RELEASE(p) if(p){(p)->Release(); (p) = nullptr;}

/// Safe delete of a pointer. Note that it is OK to delete a null
/// pointer. Note the brackets and braces.

#define SAFE_DELETE(p) {delete (p); (p) = nullptr;}

/// \brief Billboard vertex structure. 
///
/// Custom vertex format for representing a vanilla billboard object. Consists 
/// of position, and texture coordinates.

struct BILLBOARDVERTEX{ 
  Vector3 p; ///< Position.
  float tu; ///< Texture U coordinate.
  float tv; ///< Texture V coordinate.
}; //BILLBOARDVERTEX

/// Constant buffer for use by shaders.

struct ConstantBuffer{
  XMFLOAT4X4 wvp; ///< World View Projection matrix
  float u0, u1, v0, v1; // these are for animation
}; //ConstantBuffer
