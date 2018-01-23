# Audio out

This class is c++ wrapper to [BASS audio library](https://www.un4seen.com) with some often requested functions, such as:
* load sound in memory and play it when needs;
* manage loaded sounds;
* repeat playing of song;

# Usage on linux:

1. Download BASS audio library from [this site](http://www.un4seen.com/download.php?bass24-linux)
2. Unpack it in temporary directory
3. Place dynamic library (usually named `libbass.so`) in the system user library directory

```bash
  $ wget http://www.un4seen.com/download.php?bass24-linux
  $ unzip bass24-linux.zip -d basstmp/
  $ cd basstmp/x64/mp3free/
  $ sudo cp libbass.so /usr/local/lib/
  $ sudo chmod a+rx /usr/local/lib/libbass.so 
  $ sudo ldconfig
```

3. Place header file (usually named `bass.h`) in include directory of yours application (preffered `3rdparty` subdir of `AudioOut` source dir).
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
