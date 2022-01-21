#include "rpc/server.h"
#include "seal/seal.h"
#include "seal_params.h"
#include "env.h"

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <vector>

seal::BatchEncoder* encoder;
seal::Evaluator* evaluator;
seal::parms_id_type parm_id;

// Find better way to set these; probably .env file or something along those lines
const int NUM_NTT_THREADS = 5;
const int NUM_PIR_THREADS = 5;
const int DB_ROWS = 5;

// Probably don't need a mutex for queries in the future once dialing is implemented, since
// users can't change queries arbitrarily
std::mutex query_m;
std::vector<std::vector<seal::Ciphertext>> queries;

char* raw_db;
std::vector<seal::Plaintext> encoded_db;

bool preprocessing_ready;
std::mutex pp_m;
std::condition_variable pp_cv;

std::atomic<int> ntt_threads_done(0);

bool pir_ready;
std::mutex pir_m;
std::condition_variable pir_cv;

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

std::pair<int, int> get_work_chunk(int thread_id, int total_work, int num_threads) {
    int chunk_size = total_work / num_threads;
    int start = thread_id * chunk_size;
    int rem = total_work % num_threads;
    if (rem > thread_id) {
        start += thread_id;
        chunk_size++;
    } else {
        start += rem;
    }
    int end = std::min(start + chunk_size, total_work);
    return {start, end};
}

void preprocess_db(int id) {
    auto [start, end] = get_work_chunk(id, DB_ROWS, NUM_NTT_THREADS);
    std::vector<uint64_t> db_element(N);
    int round = 1; 
    for (;;) {
        std::unique_lock lock(pp_m);
        pp_cv.wait(lock, [] { return preprocessing_ready; });
        
        for (int i = start; i < end; i++) {
            for (int j = 0; j < N; j++) {
                // don't understand the point of this?
                db_element[j] = raw_db[j & ((MSG_SIZE * NUM_USERS) - 1)] + i + round * 7;
            }
            encoded_db[i].release();
            encoder->encode(db_element, encoded_db[i]);
            evaluator->transform_to_ntt_inplace(encoded_db[i], parm_id);
        }
        round++;
        if (++ntt_threads_done == NUM_NTT_THREADS) {
            ntt_threads_done = 0; 
            preprocessing_ready = false;
            std::scoped_lock lock(pir_m);
            pir_ready = true;
            pir_cv.notify_all();
        }
    }
}

void pir(int id) {
    auto [start, end] = get_work_chunk(NUM_USERS, NUM_PIR_THREADS);
    
}

int main() {
    raw_db = new char[MSG_SIZE * NUM_USERS];
    preprocessing_ready = false;
    pir_ready = false;
    encoded_db.resize(DB_ROWS);

    seal::EncryptionParameters params(seal::scheme_type::bfv); 
    params.set_poly_modulus_degree(N);
    params.set_coeff_modulus({COEFF_MODULUS_54, COEFF_MODULUS_55});
    params.set_plain_modulus(PLAIN_MODULUS);
    
    seal::SEALContext context(params);

    encoder = new seal::BatchEncoder(context);
    evaluator = new seal::Evaluator(context);
    parm_id = context.first_parms_id();
}