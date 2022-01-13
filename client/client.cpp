#include "rpc/client.h"

#include <iostream>
#include <thread>

using namespace std::chrono_literals;

int main(int argc, char** argv) { 
    if (argc != 4) {
        std::cout << "Expected EC2 instance public IP and RPC port, as well as client ID" << std::endl;
        exit(1); 
    }
    
    const char* SERV_IP = argv[1];
    const int SERV_PORT = std::stoi(argv[2]);
    const int CLIENT_ID = std::stoi(argv[3]);
    rpc::client client(SERV_IP, SERV_PORT);
    for (int i = 0; i < 5; i++) {
        client.call("send_msg", CLIENT_ID, "hello from " + std::to_string(CLIENT_ID) + " (round " + std::to_string(i) + ")");
        std::this_thread::sleep_for(3s);
    }
    return 0; 
}
