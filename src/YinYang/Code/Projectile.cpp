#include "Projectile.h"
#include "Object.h"

#include "defines.h"

void CProjectile::Throw(Vector3 Pos, Vector3 Vel) {
  m_vPos = Pos;
  m_vVel = Vel;
  m_bInAir = TRUE;
}

void CProjectile::Reset(void) {
  m_vPos = Home;
  m_vVel = Vector3(0.0f, 0.0f, 0.0f);
  m_bIsStuck = FALSE;
  m_bInAir = FALSE;
}