#include <iostream>
#include "rpc/server.h"

double add(double a, double b) { return a + b; }

int main(int argc, char** argv) { 
    if (argc < 2) {
        std::cout << "Missing port" << std::endl;
        exit(1);
    }
    
    const int PORT = std::stoi(argv[1]);
    rpc::server srv(PORT); 
    srv.bind("add", &add);
    srv.run();
    return 0;
}
