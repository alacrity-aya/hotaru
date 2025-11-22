#include <data/log_record.hpp>
#include <index/index.hpp>
#include <parallel_hashmap/btree.h>
#include <shared_mutex>
#include <vector>

namespace indexer {

using data::LogRecordPos;
using phmap::btree_map;

class BTree final: public Indexer {
public:
    bool put(std::vector<uint8_t> key, LogRecordPos pos) final;
    std::optional<LogRecordPos> get(std::vector<uint8_t>) final;
    bool del(std::vector<uint8_t> key) final;
    BTree() = default;
    ~BTree() final = default;

private:
    btree_map<std::vector<uint8_t>, LogRecordPos> tree;
    std::shared_mutex mtx;
};

} // namespace indexer
