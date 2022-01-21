#include "rpc/server.h"
#include "seal/seal.h"
#include "seal_params.h"

#include <condition_variable>
#include <mutex>
#include <vector>

const int NUM_THREADS = 5;

// Probably don't need a mutex for queries in the future once dialing is implemented, since
// users can't change queries arbitrarily
std::mutex query_m;
std::vector<std::vector<seal::Ciphertext>> queries;

char* raw_db;

bool preprocessing_ready;
std::mutex pp_m;
std::condition_variable pp_cv;

// Deserialize the serialized queries and store them in the queries map
// Not sure if deserialization time is too expensive, maybe look into 
// 1. making each thread deserialize its own query (similar to how each thread NTT transforms its own query)
// 2. sending full ciphertext instead of serializing in the first place
void update_queries(
    std::vector<std::vector<std::vector<seal::seal_byte>>> serialized_queries) {
    std::scoped_lock lock(query_m);
    std::transform(serialized_queries.begin(), serialized_queries.end(),
                   queries.begin(), [](const auto& serialized_query) {
                       std::vector<seal::Ciphertext> deserialized_query(
                           serialized_query.size());
                       for (int i = 0; i < serialized_query.size(); i++) {
                           deserialized_query[i].load(
                               reinterpret_cast<const seal_byte*>(
                                   serialized_query[i].data()),
                               serialized_query[i].size());
                       }
                       return deserialized_query;
                   });
}

std::vector<std::vector<uint64_t>> get_answers(std::string db) {
    // No lock needed; server initiates this
    raw_db = db.data();
    {
        std::scoped_lock lock(pp_m);
        preprocessing_ready = true;
        pp_cv.notify_all();
    }
}

int main() {
    preprocessing_ready = false;
}