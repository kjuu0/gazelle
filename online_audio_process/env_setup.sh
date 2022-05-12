#!/bin/bash

sudo apt-get update
sudo apt-get -y install g++
sudo apt-get -y install libssl-dev
sudo apt-get install -y build-essential autoconf libtool pkg-config

# gazelle project
sudo apt-get install git
sudo apt-get install libasound-dev
sudo apt-get install libtool

pushd ~
wget http://files.portaudio.com/archives/pa_stable_v190700_20210406.tgz
tar -xzvf pa_stable_v190700_20210406.tgz
pushd portaudio
./configure && make
sudo make install
popd

git clone https://github.com/xiph/LPCNet.git
pushd LPCNet
./autogen.sh
./configure
make
export CFLAGS='-Ofast -g -march=native'
sudo make install
popd

# sudo apt-get install unzip
# wget https://github.com/missmiss20/gazelle/archive/refs/heads/master.zip
# unzip master.zip
# gazelle project end

wget https://github.com/Kitware/CMake/releases/download/v3.18.4/cmake-3.18.4.tar.gz
tar -xzvf cmake-3.18.4.tar.gz
pushd cmake-3.18.4
./bootstrap
./bootstrap
make -j4
sudo make install
sudo ln -s /usr/local/bin/cmake /usr/bin/cmake
popd
sudo apt-get -y install clang

git clone https://github.com/microsoft/SEAL.git
pushd SEAL
git checkout 3.5.6
git pull
cmake . -DBUILD_SHARED_LIBS=ON -DCMAKE_VERBOSE_MAKEFILE=ON -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++
make
sudo make install
popd

git clone https://github.com/rpclib/rpclib.git
pushd rpclib
cmake .
make
sudo make install
popd

popd
