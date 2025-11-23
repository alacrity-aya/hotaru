#include "fio/file_io.hpp"
#include <index/btree.hpp>
#include <index/index.hpp>
#include <memory>

int main(int argc, char* argv[]) {
    fio::FileIO::ptr file = std::make_shared<fio::FileIO>(1);
    file->write({});
}
