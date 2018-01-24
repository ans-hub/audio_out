// *************************************************************
// File:    audio_out.h
// Descr:   wrapper to BASS audio library
// Author:  Novoselov Anton @ 2017-2018
// URL:     https://github.com/ans-hub/audio_out
// *************************************************************

// BASS Library docs placed here: https://www.un4seen.com/doc/

#include "audio_out.h"

namespace anshub {

AudioOut::AudioOut()
  : inited_{false}
  , loaded_{ }
  , channels_cnt_{audio_helpers::kChannelsCount}
{
  if (BASS_Init(-1, 44100, BASS_DEVICE_DEFAULT, 0, 0))
    inited_ = true;
  else
    audio_helpers::PrintBassError();
}

AudioOut::~AudioOut()
{
  for (auto& a : loaded_)
    BASS_SampleFree(a.second);
  if (!inited_)
    BASS_Free();
}

// Plays the sample (with loading its before)

bool AudioOut::Play(const std::string& fname, bool repeat)
{
  Handle hndl = Load(fname, repeat);
  if (!hndl)
    return audio_helpers::PrintBassError();  

  hndl = BASS_SampleGetChannel(hndl, FALSE);
  BASS_ChannelSetAttribute(hndl, BASS_ATTRIB_VOL,0.5f);

  if (BASS_ChannelPlay(hndl, FALSE)) 
    return true;
  else
    return audio_helpers::PrintBassError();
}

// Stops playing the sample in two ways - by immediately and by
// set loop flag to 0, therefore next loop is not come

bool AudioOut::Stop(const std::string& fname, bool immediately)
{
  Handle hndl = FindLoaded(fname);
  
  if (!hndl)
    return audio_helpers::PrintBassError();

  if (immediately)
    return StopPlayingImmediately(hndl);
  else 
    return RemoveLoopFromSample(hndl);
}

// Returns filename of first finded sample which currently is playing

AudioOut::VStrings AudioOut::NowPlaying(bool only_repeated) const
{
  VStrings res{};
  
  for (const auto& l : loaded_)
  {
    auto sample_hndl = l.second;
    auto channels_hndls = GetLoadedChannels(sample_hndl);
    
    if (IsChannelsPlayingNow(channels_hndls)) { 
      if (!only_repeated)
        res.push_back(l.first);
      else
        if (audio_helpers::IsRepeatedSample(sample_hndl)) res.push_back(l.first);
    }
  }
  return res;
}

// IMPLEMENTATION DETAILS

// Loads sample by filename, saves to loaded_ vector and return its handle

AudioOut::Handle AudioOut::Load(const std::string& fname, bool repeat)
{
  Handle hndl = FindLoaded(fname);

  if (!hndl) {
    DWORD flags;
    if (repeat) 
      flags = BASS_SAMPLE_LOOP;
    else
      flags = BASS_SAMPLE_8BITS + BASS_SAMPLE_OVER_POS;

    hndl = BASS_SampleLoad(FALSE, fname.c_str(), 0,0,3, flags);
    loaded_.push_back(std::make_pair(fname, hndl));
  }
  if (!hndl)
    audio_helpers::PrintBassError();

  return hndl;
}

// Returns handle of sample if its were been loaded early

AudioOut::Handle AudioOut::FindLoaded(const std::string& fname) const
{
  for (auto& l:loaded_)
    if (l.first == fname) return l.second;
  return 0;
}

// Gets vector of handles to all loaded channels of sample

AudioOut::VHandles AudioOut::GetLoadedChannels(const Handle& hndl) const
{
  // Create struct to save channels list
  
  auto  nfo = audio_helpers::GetSampleInfo(hndl);
  auto  ch_list_ptr = std::unique_ptr<HCHANNEL>{new HCHANNEL[nfo.max]};
  auto* ch_list = ch_list_ptr.get();
  
  // Get channels list
  
  DWORD ch_count {0};
  ch_count  = BASS_SampleGetChannels(hndl, ch_list);
  
  // Save channels to vector
  
  VHandles res{};
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

// Stop playback of given sample immediately

bool AudioOut::StopPlayingImmediately(const Handle& hndl)
{
  HCHANNEL ch = BASS_SampleGetChannel(hndl, FALSE);    
  BASS_ChannelSetPosition(ch, 0, BASS_POS_BYTE);
  return BASS_SampleStop(hndl);
}

// Stop playback after end of playing sample and change loop flag 

bool AudioOut::RemoveLoopFromSample(const Handle& hndl)
{
  if (!audio_helpers::IsRepeatedSample(hndl)) 
    // return StopPlayingImmediately(hndl);
    return false;
  
  auto channels_hndls = GetLoadedChannels(hndl);
  for (auto& ch : channels_hndls) {
    if (BASS_ChannelFlags(ch, 0, 0)&BASS_SAMPLE_LOOP) {
      BASS_ChannelFlags(ch, 0, BASS_SAMPLE_LOOP);
      return true;
    }
  }
  return false;
}

// HELPERS

bool audio_helpers::IsNowPlaying(const AudioOut& audio, const std::string& fname)
{
  auto now_playing = audio.NowPlaying(false);

  for (const auto& sound : now_playing) {
    if (sound == fname) return true;
  }
  return false;
}

bool audio_helpers::StopAllNowPlaying(AudioOut& audio, bool only_repeated)
{
  bool result{true};
  auto sounds = audio.NowPlaying(only_repeated);
  
  for (const auto& sound : sounds) {
    if (!audio.Stop(sound)) result = false;
  }
  return result;
}

// Returns true if given sample is looped

bool audio_helpers::IsRepeatedSample(const Handle& hndl)
{
  auto info = audio_helpers::GetSampleInfo(hndl);  
  if (info.flags == BASS_SAMPLE_LOOP + BASS_SAMPLE_SOFTWARE)
    return true;
  else
    return false;
}

audio_helpers::SampleNfo audio_helpers::GetSampleInfo(const Handle& hndl)
{
  BASS_SAMPLE nfo;
  BASS_SampleGetInfo(hndl, &nfo);
  return nfo;
}

bool audio_helpers::SetSampleInfo(const Handle& hndl, SampleNfo& nfo)
{
  return BASS_SampleSetInfo(hndl, &nfo);
}

bool audio_helpers::PrintBassError()
{
  auto err = BASS_ErrorGetCode();

#ifdef DEBUG
  std::cerr << "Audio module: libbass.so error code: " << err << '\n'
    << "  More at: http://www.un4seen.com/doc/#bass/BASS_ErrorGetCode.html" << '\n';
#endif

  return err ? false : true; 
}

}  // namespace anshuv