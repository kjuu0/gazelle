# PortAudio Build Steps

## Dependencies
- [portaudio](http://portaudio.com/): For Voice Call I/O

## Build

From `gazelle/test-portaudio`:
```
mkdir build
cd build
cmake ..
make
```

## Usage
Executables located in `gazelle/test-portaudio/build/bin`:
```
./echo
./paex_record
./beep
```
echo takes in whatever in input device captures and output it immidiately using the default output device,
use earphones to prevent a positive feedback loop.

paex_record will record 5 seconds, then replay it, it has the functionaly to save the recording in a .raw file as well.

beep produces a constant buzz voice.