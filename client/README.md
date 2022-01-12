# Gazelle Client 

Currently, the client just establishes a connection with an RPC that implements a function `add` and prints the result.

## Dependencies

- [rpclib](https://github.com/rpclib/rpclib): for communicating with the server using RPC

## Build

From `gazelle/client`:
```
mkdir build
cd build
cmake ..
make
```

The client executable will be located in `gazelle/client/build/bin/gazelle_client`.

## Usage

`./gazelle_client <server_ip> <server_port>`

Expected output should be `5`.