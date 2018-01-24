# Audio out

This class is c++ wrapper to [BASS audio library](https://www.un4seen.com) with some often requested functions, such as:
* load sound in memory and play it when needs;
* manage loaded sounds;
* repeat playing of song;

# Usage on linux:

1. Download BASS audio library from [this site](http://www.un4seen.com/download.php?bass24-linux)
2. Unpack it in temporary directory
3. Place dynamic library (usually named `libbass.so`) in the system user library directory (usually /usr/local/lib/)

```bash
  $ wget http://us.un4seen.com/files/bass24-linux.zip
  $ unzip bass24-linux.zip -d tmp/
  $ sudo cp tmp/x64/libbass.so /usr/local/lib/  # for 32-bit - tmp/libbass.so
  $ sudo chmod a+rx /usr/local/lib/libbass.so
  $ sudo ldconfig
  $ rm -rf tmp/ bass24-linux.zip
```

P.S.: if something goes wrong while downloading bass audio library, then the best way is to use [direct link](https://www.un4seen.com/bass.html) for downloading library

3. Place header file (usually named `bass.h`) in include directory of yours application (preffered `3rdparty` subdir of `AudioOut` source dir).
4. If the directory path mentioned above would be equal to a directory path where placed this class AudioOut this would be perfect, since you do not needs to make changes in class `AudioOut`
5. Then we should make static library simple by typing `make` inside `AudioOut` directory.
6. In header file of yours application You should include `audio_out.h`.
7. In Makefile of application we should write path to AudioOut static library (-L flag) and compiler flags: -laudio -lbass

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
