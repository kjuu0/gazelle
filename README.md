# Gazelle


## Offline audio process
In this folder, you can create pcm recordings and playback pcm files using portaudio

```
cd offline_audio_process
mkdir build
cd build
cmake ..
make
cd bin
```

run paex_record will record 2 seconds of pcm data, the data will be stored in a file called input.pcm
```
./paex_record 
```

run playback_record will playback the audio stored in a file called output.pcm (if file exists)
```
./playback_record 
```

## online audio process


2.
Set up the environment
```
cd online_audio_process
./env_setup.sh

See the README.md in the online_audio_process folder (follow Addra setup)
```

## Tips on how to build
1.
Gazelle works on Linux environment only
Virtual box set up Linux virtual machine:
https://www.youtube.com/watch?v=mfPNI2SBOKk&t=1s


2.
If portaudio or Lpcnet is not successfully installed, try installing them manually.

Portaudio/
http://files.portaudio.com/download.html
```
pushd ~/Downloads
wget pa_stable_v190700_20210406.tgz
tar -xzvf pa_stable_v190700_20210406.tgz
pushd portaudio
./configure && make
autoconf
autoreconf --install
./configure && make
sudo make install
popd
popd
```

lpcnet/
https://github.com/xiph/LPCNet
```
pushd ~/Downloads
git clone https://github.com/xiph/LPCNet.git
pushd LPCNet
./autogen.sh
./configure
make
export CFLAGS='-Ofast -g -march=native'
sudo make install
popd
popd
```
