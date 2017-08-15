// Package: bass_wrapper(v0.22)
// Description: https://github.com/ans-hub/bass_wrapper
// Author: Anton Novoselov, 2017
// File: class that represents wrapper to BASS audio library

#include "audio_out.h"

namespace wumpus_game {

AudioOut::AudioOut()
  : inited_{false}
  , loaded_{ }
{
  if (BASS_Init(-1, 44100, BASS_DEVICE_DEFAULT, 0, 0))
    inited_ = true;
}

AudioOut::~AudioOut()
{
  for (auto& a : loaded_) BASS_SampleFree(a.second);
  if (!inited_) BASS_Free();
}

// Plays the sample (with loading its before)

bool AudioOut::Play(const std::string& fname, bool repeat)
{
  Handle hndl = Load(fname, repeat);
  if (!hndl) return false;

  hndl = BASS_SampleGetChannel(hndl, FALSE);
  BASS_ChannelSetAttribute(hndl, BASS_ATTRIB_VOL,0.5f);

  if (BASS_ChannelPlay(hndl, FALSE)) 
    return true;
  return false;
}

// Stops playing the sample

bool AudioOut::Stop(const std::string& fname)
{
  Handle hndl = FindLoaded(fname);
  if (hndl) {
    HCHANNEL ch = BASS_SampleGetChannel(hndl, FALSE);    
    BASS_ChannelSetPosition(ch, 0, BASS_POS_BYTE);
    return BASS_SampleStop(hndl);
  }
  return false;
}

// Returns filename of first finded sample which currently is playing

AudioOut::VStrings AudioOut::NowPlaying(bool only_repeated) const
{
  VStrings res{};
  
  for (const auto& l : loaded_) {

    auto sample_hndl = l.second;
    auto channels_hndls = GetLoadedChannels(sample_hndl);
    
    if (IsChannelsPlayingNow(channels_hndls)) { 
      if (!only_repeated)
        res.push_back(l.first);
      else
        if (IsRepeatedSample(sample_hndl)) res.push_back(l.first);
    }
  }
  return res;
}

// REALISATION DETAILS

// Loads sample by filename, saves to loaded_ vector and return its handle

AudioOut::Handle AudioOut::Load(const std::string& fname, bool repeat)
{
  Handle hndl = FindLoaded(fname);
  if (!hndl) {
    hndl = BASS_SampleLoad(FALSE, fname.c_str(), 0,0,3, repeat ? 4 : 1 );
    loaded_.push_back(std::make_pair(fname, hndl));
  }
  return hndl;
}

// Returns handle of sample if its were been loaded early

AudioOut::Handle AudioOut::FindLoaded(const std::string& fname) const
{
  for (auto& l:loaded_) {
    if (l.first == fname) return l.second;
  }
  return 0;
}

// Returns true if given sa,ple is looped

bool AudioOut::IsRepeatedSample(const Handle& hndl) const
{
  BASS_SAMPLE info;
  BASS_SampleGetInfo(hndl, &info);
  if (info.flags == BASS_SAMPLE_LOOP + BASS_SAMPLE_SOFTWARE)
    return true;
  else
    return false;
}

// Gets vector of handles to all loaded channels of sample

AudioOut::VHandles AudioOut::GetLoadedChannels(const Handle& hndl) const
{
  VHandles res{};

  BASS_SAMPLE info;
  BASS_SampleGetInfo(hndl, &info); 

  auto ch_list_ptr = std::unique_ptr<HCHANNEL>{new HCHANNEL[info.max]};
  HCHANNEL* ch_list = ch_list_ptr.get();

  DWORD ch_count {0};
  ch_count  = BASS_SampleGetChannels(hndl, ch_list);
  for (DWORD i = 0; i < ch_count; ++i) {
    res.push_back(ch_list[i]);
  }
  return res;
}

// Returns true if at least one channel is playing now

bool AudioOut::IsChannelsPlayingNow(const VHandles& handles) const
{
  for (const auto& h : handles) {
    auto active = BASS_ChannelIsActive(h);
    if (active) return true;
  }
  return false;
}

// HELPERS

bool audio_helpers::StopAllNowPlaying(AudioOut& audio, bool only_repeated)
{
  bool result{true};
  auto sounds = audio.NowPlaying(only_repeated);
  
  for (const auto& sound : sounds) {
    if (!audio.Stop(sound)) result = false;
  }
  return result;
}

bool audio_helpers::IsNowPlaying(const AudioOut& audio, const std::string& fname)
{
  auto now_playing = audio.NowPlaying(false);

  for (const auto& sound : now_playing) {
    if (sound == fname) return true;
  }
  return false;
}

}  // namespace wumpus_game