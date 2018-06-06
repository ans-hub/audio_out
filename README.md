# Audio out

This class is c++ wrapper to [BASS audio library](https://www.un4seen.com) with some often requested functions, such as:
* load sound in memory and play it when needs;
* manage loaded sounds;
* repeat playing of song;
* change tempo and pich

# Usage on linux:

1. Download BASS audio library from [this link] (http://us.un4seen.com/files/bass24-linux.zip)
2. Unpack it in temporary directory
3. Place shared library (usually `libbass.so` from `mp3-free` subdir) in the system user library directory (usually /usr/local/lib/)
4. Install BASS_FX addon (`libbass_fx.so`) - is the same as above (http://www.un4seen.com/files/z/0/bass_fx24-linux.zip)

```bash
  $ wget http://us.un4seen.com/files/bass24-linux.zip
  $ unzip bass24-linux.zip -d tmp/
  $ sudo cp tmp/x64/libbass.so /usr/local/lib/  # for 32-bit - tmp/libbass.so
  $ sudo chmod a+rx /usr/local/lib/libbass.so
  $ sudo ldconfig
  $ rm -rf tmp/ bass24-linux.zip
```

Installing libbass_fx.so - is the same as above

P.S.: if something goes wrong while downloading bass audio library, then the best way is to use [direct link](https://www.un4seen.com/bass.html) for downloading library

5. Place header files (usually named `bass.h` and `bass_fx.h`) into include directory of yours application (preffered `3rdparty` subdir of `AudioOut` source dir).
6. Then we should make static library simple by the following:
```
  $ mkdir build
  $ cd build
  $ cmake ../
  $ make
```
7. In header file of yours application You should include `audio_out.h` or `audio_fx.h.
8. In Makefile of application we should write path to AudioOut static library (-L flag) and compiler flags: -laudio -lbass -laudio_fx -lbass_fx

# Usage example:

### Once creating of instance of class:
```c++
  try {
    AudioOut audio;
  }
  catch (...) {
    ...
  }
```

### Playing looped sound: 
```c++
  audio.Play("some_song.wav", true);
```

### Stop looped sound:
```c++
  audio.Stop("some_song.wav"); 
```

### Stop looped sound after ending current sound:
```c++
  audio.Stop("some_song.wav", true);
```

### Stop all now-playing sounds:
```c++
  AudioOut& audio;
  ...
  audio_helpers::StopAllNowPlayingSong(audio, true);
```

### To know now playing repeated sound:
```c++
  std::vector<std::string> playing = audio.NowPlaying(true);
```

### Usage of Audio_FX
```c++

  // Prepare audio sound represents engine of a boat

  AudioFx audio {};
  auto engine_snd = "snd_engine.wav";
  auto engine_mod = AudioFx::Modifier::PITCH;

  // Show to AudioFx that we want to change pitch in the future in the particular range

  auto min_pitch = 0.2f;
  auto max_pitch = 1.0f;
  audio.LoadFx(engine_snd, true);
  audio.SetModifierRange(engine_snd, engine_mod, min_pitch, max_pitch);
  audio.SetModifierValue(engine_snd, engine_mod, min_pitch);

  audio.PlayFx(engine_snd);
  
  while (true) {
  ...
    // Somewhere in game loop - process player sounds

    auto range_1  = std::make_pair(0.0f, boat.GetDynamics().GetMaxSpeed());
    auto range_2  = audio.GetModifierRange(engine_snd, engine_mod);
    auto curr_vel = boat.GetDynamics().GetCurrentSpeed();
    auto modifier_val = math::LeadToRange(range_1, range_2, curr_vel);
    audio.SetModifierValue(engine_snd, engine_mod, modifier_val);

  ..
  }
```
