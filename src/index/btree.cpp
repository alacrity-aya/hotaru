#include <index/btree.hpp>
#include <mutex>
#include <shared_mutex>
#include <utils.hpp>
#include <vector>

namespace indexer {

bool BTree::put(std::vector<uint8_t> key, LogRecordPos pos) {
    std::unique_lock<std::shared_mutex> lock(this->mtx);
    this->tree.emplace(key, pos);
    return true;
}

std::optional<LogRecordPos> BTree::get(std::vector<uint8_t> key) {
    std::shared_lock<std::shared_mutex> lock(this->mtx);
    if (this->tree.find(key) != this->tree.end()) {
        return this->tree[key];
    }
    return std::nullopt;
}

bool BTree::del(std::vector<uint8_t> key) {
    std::unique_lock<std::shared_mutex> lock(this->mtx);
    return this->tree.erase(key) != 0;
}

} // namespace indexer
