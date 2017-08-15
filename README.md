# Audio out

This class is wrapper to [BASS audio library](https://www.un4seen.com) with some often requested functions, such as:
* load sound in memory and play it when needs;
* manage loaded sounds;
* repeat playing of song;

# Usage on linux:

1. Download BASS audio library from [this site](https://www.un4seen.com)
2. Place dynamic library (usually named `libbass.so`) and header file (usually named `bass.h`) in the library directory of yours application.
3. Say to compilar where to search library path and connect library:
```make
clang++ -L libdir -lbass
```
4. If pathes mentioned above would be equal path where placed this class `AudioOut` this would be perfect, since you do not needs to make changes in class `AudioOut` 
5. In header file of yours application include `audio_out.h` and use it.

# Usage example:

** Playing looped sound: 
```c++
  AudioOut audio;
  audio.Play("some_song.wav", true);
```

** Stop looped sound:
```c++
  audio.Stop("some_song.wav"); 
```

** To know now playing repeated sound:
```c++
  std::vector<std::string> audio.NowPlaying(true);
```