#include "rpc/server.h"
#include "seal/seal.h"
#include "env.h"

#include <condition_variable>
#include <iostream>
#include <mutex>
#include <string>

bool all_msgs_received;
size_t msg_count;
std::mutex msg_m;
std::condition_variable msg_cv;

char* raw_db;
std::vector<std::vector<std::vector<seal::seal_byte>>> query_map;

// authorization? verify that user sending isn't just mocking some other user...
void send_msg(int client_id, std::string msg) {
    std::cout << "received msg: " << msg << " from client: " << client_id
              << std::endl;
    memcpy(raw_db + client_id * MSG_SIZE, msg.c_str(), MSG_SIZE);
    std::scoped_lock lock(msg_m);
    if (++msg_count == NUM_USERS) {
        all_msgs_received = true;
        msg_count = 0;
        msg_cv.notify_one();
    }
}

void connect_with(int client_id,
                  std::vector<std::vector<seal::seal_byte>> query) {
    // Current possible race condition if a thread is reading the map to send the 
    // query to the worker while the client is changing who they're connecting to...
    // When calling phase is established this shouldn't happen, since 
    // the server fetches every client's query at the beginning of the round.
    // This function should be removed when calling phase is implemented.
    query_map[client_id] = std::move(query);
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Missing port" << std::endl;
        exit(1);
    }

    all_msgs_received = false;
    msg_count = 0;
    query_map.resize(NUM_USERS);

    raw_db = new char[MSG_SIZE * NUM_USERS];
    memset(raw_db, '\0', MSG_SIZE * NUM_USERS);

    const int PORT = std::stoi(argv[1]);
    rpc::server srv(PORT);
    srv.bind("send_msg", send_msg);
    srv.async_run();
    for (;;) {
        std::unique_lock lock(msg_m);
        // should probably add timeout, in case users go down/poor connection
        // and don't send anything
        msg_cv.wait(lock, [] { return all_msgs_received; });
        all_msgs_received = false;

        for (int i = 0; i < MSG_SIZE * NUM_USERS; i++)
            std::cout << raw_db[i];
        std::cout << std::endl;
        // dunno wtf this does...
        // raw_db[i] = 100 + current_round * 5 + (i & 127);
    }
    return 0;
}
