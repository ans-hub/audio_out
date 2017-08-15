// Package: bass_wrapper(v0.2)
// Description: https://github.com/ans-hub/bass_wrapper
// Author: Anton Novoselov, 2017
// File: class that represents wrapper to BASS audio library

#ifndef AUDIO_OUT_H
#define AUDIO_OUT_H

#include <vector>
#include <string>
#include <memory>

#include "3rdparty/bass.h"

namespace wumpus_game {

class AudioOut
{
public:

  using Handle    = HSAMPLE;
  using FileName  = std::string;
  using VHandles  = std::vector<Handle>;
  using VSounds   = std::vector<std::pair<FileName, Handle>>;
  using VStrings  = std::vector<std::string>;

  AudioOut();
  ~AudioOut();

  bool      Play(const FileName&, bool repeat = false);
  bool      Stop(const FileName&);
  Handle    Load(const FileName&, bool);
  VStrings  NowPlaying(bool only_repeated) const;

private:

  bool      inited_;    // flag to show is bass lib is inited
  VSounds   loaded_;    // currently loaded samples in memory 
  
  Handle    FindLoaded(const FileName&) const;
  bool      IsRepeatedSample(const Handle&) const;
  VHandles  GetLoadedChannels(const Handle&) const;
  bool      IsChannelsPlayingNow(const VHandles&) const;
  
};

}  // namespace wumpus_game

#endif  // AUDIO_OUT_H