#include <index/btree.hpp>
#include <index/index.hpp>
#include <print>
#include <vector>

int main(int argc, char* argv[]) {
    std::println("hotora");

    auto btree = indexer::BTree();
    std::vector<uint8_t> vec { 1, 1, 1 };
    std::println("get = {}", btree.get(vec).has_value());
    std::println("put = {}", btree.put(vec, { .file_id = 1, .offset = 1 }));
    std::println("get = {}", btree.get(vec).has_value());
    std::println("del = {}", btree.del(vec));
    std::println("del = {}", btree.del(vec));
}
