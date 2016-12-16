/// \file ai.cpp
/// \brief Code for the intelligent object class CIntelligentObject.

#include "ai.h"
#include "debug.h"
#include "Object.h"
#include "Ninja.h"

extern CNinja* g_pNinja;
extern int g_nScreenWidth;

/// Constructor for intelligent object.
/// \param object Object type.
/// \param name Object name string.
/// \param location Vector location in world space.
/// \param velocity Velocity vector.

CIntelligentObject::CIntelligentObject(ObjectType object, const Vector3& location,
	const Vector3& velocity, C3DSprite *sprite) :
CGameObject(object, location, velocity, sprite){ //constructor
  m_fDistance = m_fXDistance = m_fYDistance = 0.0f;
  m_vNinjaLoc.x = m_vNinjaLoc.y = 0.0f;
} //constructor

/// Compute the distance to the plane. Intelligent objects need to make
/// decisions based on how close the plane is.

void CIntelligentObject::think(){
	if (g_pNinja->getDetected()) {
		Vector3 v = g_pNinja->getPos() - getPos();
		if (v.Length() < g_nScreenWidth / 1.5f) {
			m_bNinjaDetect = TRUE;
		}
		
		//look for ninja
		m_vNinjaLoc = g_pNinja->getPos(); //remember plane location

		//Euclidean and axial distances from ninja
		m_fYDistance = fabs(m_vPos.y - m_vNinjaLoc.y); //vertical distance

		//horizontal distance
		m_fXDistance = fabs(m_vPos.x - m_vNinjaLoc.x);
		//Euclidean distance
		m_fDistance = sqrt(m_fXDistance*m_fXDistance + m_fYDistance*m_fYDistance);
	}
	else m_bNinjaDetect = FALSE;
} //think