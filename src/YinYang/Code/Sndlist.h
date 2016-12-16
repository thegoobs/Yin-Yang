/// \file sndlist.h
/// \brief Enumerated types for sounds.

#pragma once

/// \brief Game sound enumerated type. 
///
/// Sounds must be listed here in the same order that they
/// are in the sound settings XML file.

enum GameSoundType{ 
  CAW_SOUND, GUN_SOUND, BOOM_SOUND, 
  THUMP_SOUND, PLANE_SOUND, TRUCK_SOUND, STINGS_SOUND, SMARTS_SOUND
}; //GameSoundType
