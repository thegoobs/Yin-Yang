/// \file ai.h
/// \brief Interface for the intelligent object class CIntelligentObject.

#pragma once

#include "object.h"

/// \brief The intelligent object class.
///
/// This is the base class for our artificial intelligence. It maintains
/// the functionality that all intelligent objects have in common, that is,
/// knowledge of the ninja's location.

class CIntelligentObject: public CGameObject{
  protected:
    Vector3 m_vNinjaLoc; ///< Ninja location.
	BOOL m_bNinjaDetect;
    float m_fDistance; ///< Euclidean distance to ninja.
    float m_fYDistance; ///< Vertical distance to ninja.
    float m_fXDistance; ///< horizontal distance to ninja.

  public:
    CIntelligentObject(ObjectType object, const Vector3& location,
      const Vector3& velocity, C3DSprite *sprite); ///< Constructor.
    virtual void think(); ///< AI function.
}; //CIntelligentObject