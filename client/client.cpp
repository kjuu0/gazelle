#include <iostream>
#include "rpc/client.h"

int main(int argc, char** argv) { 
    if (argc != 3) {
        std::cout << "Expected EC2 instance public IP and RPC port" << std::endl;
        exit(1); 
    }
    
    const char* SERV_IP = argv[1];
    const int SERV_PORT = std::stoi(argv[2]);
    rpc::client client(SERV_IP, SERV_PORT);
    double res = client.call("add", 2, 3).as<double>();
    std::cout << res << std::endl;
    return 0; 
}
