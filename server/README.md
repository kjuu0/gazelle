# Gazelle Server

Currently, the server just implements an add function and launches an RPC server that handles add calls from clients.

## Dependencies

- [rpclib](https://github.com/rpclib/rpclib): for answering queries using RPC

## Build

From `gazelle/server`:
```
mkdir build
cd build
cmake ..
make
```

The client executable will be located in `gazelle/client/build/bin/gazelle_server`.

## Usage

`./gazelle_server <port>`

## Hosting on AWS EC2

Some barebones, basic steps to hosting the server (probably not securely) on an AWS EC2 instance:

1. Create a new AWS EC2 instance with default/desired config values
2. `Configure security groups` and add an inbound `Custom TCP` rule, and specify any port (outside of the defaults like `80` or `443`) that the RPC server will be listening on
3. Create or use an existing key pair and save it somewhere
4. Build `gazelle_server` targeting whatever OS your EC2 instance is (@kjuu0: personally used Ubuntu 20.04, maybe we'll Dockerize this later)
5. Copy the executable to your EC2 instance, something like: `scp -i <path_to_key_pair> <path_to_gazelle_server_bin> <user>@<public_ec2_ipv4_dns>:~/.` (copies the binary to the EC2 instance's home directory)
6. Execute the executable in background `./gazelle_server <port> &`
