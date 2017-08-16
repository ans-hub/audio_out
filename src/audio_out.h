// Package: bass_wrapper(v0.24)
// Description: https://github.com/ans-hub/audio_out
// Author: Anton Novoselov, 2017
// File: class that represents wrapper to BASS audio library

// BASS Library docs placed here: https://www.un4seen.com/doc/

#ifndef AUDIO_OUT_H
#define AUDIO_OUT_H

#include <vector>
#include <string>
#include <memory>

#include "3rdparty/bass.h"

namespace anshub {

class AudioOut
{
public:
  using Handle    = HSAMPLE;
  using SampleNfo = BASS_SAMPLE;
  using FileName  = std::string;
  using VHandles  = std::vector<Handle>;
  using VSounds   = std::vector<std::pair<FileName, Handle>>;
  using VStrings  = std::vector<std::string>;

  AudioOut();
  ~AudioOut();

  bool      Play(const FileName&, bool repeat = false);
  bool      Stop(const FileName&, bool immediately = true);
  Handle    Load(const FileName&, bool);
  VStrings  NowPlaying(bool only_repeated) const;
  
private:
  bool      inited_;    // flag to show is bass lib is inited
  VSounds   loaded_;    // currently loaded samples in memory 
  
  Handle    FindLoaded(const FileName&) const;
  VHandles  GetLoadedChannels(const Handle&) const;
  bool      IsChannelsPlayingNow(const VHandles&) const;     
  bool      StopPlayingImmediately(const Handle&);
  bool      RemoveLoopFromSample(const Handle&);
};

namespace audio_helpers {
  
  using FileName = AudioOut::FileName; 
  using SampleNfo = AudioOut::SampleNfo;
  using Handle = AudioOut::Handle;
  
  bool      IsNowPlaying(const AudioOut&, const AudioOut::FileName&);
  bool      StopAllNowPlaying(AudioOut&, bool only_repeated);

  bool      IsRepeatedSample(const Handle&);
  SampleNfo GetSampleInfo(const Handle&);
  bool      SetSampleInfo(const Handle&, SampleNfo&);

}  // namespace audio_helpers

}  // namespace anshub

#endif  // AUDIO_OUT_H