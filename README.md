# Audio out

This class is c++ wrapper to [BASS audio library](https://www.un4seen.com) with some often requested functions, such as:
* load sound in memory and play it when needs;
* manage loaded sounds;
* repeat playing of song;

# Usage on linux:

1. Download BASS audio library from [this site](https://www.un4seen.com)
2. Place dynamic library (usually named `libbass.so`) and header file (usually named `bass.h`) in the library directory of yours application.
3. Say to compiler where its would to search a libraries and connect the library `libbass.so`:
```make
clang++ -L libdir -lbass
```
4. If the directory path mentioned above would be equal to a directory path where placed this class AudioOut this would be perfect, since you do not needs to make changes in class `AudioOut`
5. In header file of yours application You should include `audio_out.h` and use it.

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