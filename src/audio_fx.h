// *******************************************************************
// File:    audio_fx.h
// Descr:   wrapper to BASS_FX (addon of BASS audio library) (v0.41)
// Author:  Novoselov Anton @ 2018
// URL:     https://github.com/ans-hub/audio_out
// *******************************************************************

// BASS Library docs placed here: https://www.un4seen.com/doc/

#ifndef AUDIO_FX_H
#define AUDIO_FX_H

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <map>
#include <cassert>
#include <functional>

#include "audio_out.h"
#include "3rdparty/bass.h"
#include "3rdparty/bass_fx.h"

namespace anshub {

struct AudioFx : public AudioOut
{
  enum class Modifier { TEMPO, PITCH, FREQ };
  
  using Handle   = DWORD;
  using String   = std::string;
  using cString  = const String;
  using P_Range  = std::pair<float, float>;
  using M_Range  = std::map<Modifier, P_Range>;

  struct SoundInfo
  {
    SoundInfo() : hndl_{}, ranges_{} { }
    SoundInfo(Handle hndl) : hndl_{hndl}, ranges_{} { }
    Handle  hndl_;
    M_Range ranges_;
  
  }; // struct SoundInfo

  using V_Sounds = std::map<std::string, SoundInfo>;

  AudioFx();
  virtual ~AudioFx() override;

  bool    LoadFx(const FileName&, bool repeat);
  bool    PlayFx(const FileName&, bool repeat = false);
  bool    StopFx(const FileName&);

  bool    SetModifierRange(const FileName&, Modifier, float min, float max);
  bool    SetModifierValue(const FileName&, Modifier, float val);
  P_Range GetModifierRange(const FileName&, Modifier) const;

private:
  V_Sounds  streams_;

  Handle    GetLoadedHandle_FX(const FileName&);

}; // class AudioFx

namespace audio_consts {

  // Ranges for given modifiers as it said in bass_fx documentation
  
  const AudioFx::M_Range kFxRanges
  {
    { AudioFx::Modifier::TEMPO, std::make_pair(-95.0f, +5000.0f) },  // percents
    { AudioFx::Modifier::PITCH, std::make_pair(-60.0f, +60.0f)   },  // semitones
    { AudioFx::Modifier::FREQ,  std::make_pair(5.0f,   +5000.0f) }   // percents (hz)
  };

} // namespace audio_consts

} // namespace anshub

#endif  // AUDIO_FX_H