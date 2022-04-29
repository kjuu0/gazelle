# Gazelle

## Build Steps
1.
Virtual box set up Linux virtual machine:
https://www.youtube.com/watch?v=mfPNI2SBOKk&t=1s

2.
```
sudo apt-get install git
sudo apt-get install libasound-dev
sudo apt-get install libtool
sudo apt-get libtool-bin
sudo apt-get install unzip
```

3.
Addra
https://github.com/ishtiyaque/Addra
```
pushd ~/Documents
pushd Addra
git clone https://github.com/ishtiyaque/Addra.git
./env_setup.sh
popd
popd
```

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

4.
gazelle
```
pushd ~/Documents
wget https://github.com/missmiss20/gazelle.git
unzip gazelle-master.zip
popd
```


