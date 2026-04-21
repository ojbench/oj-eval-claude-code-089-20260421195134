#include <iostream>
#include "src.hpp"
#include <cassert>

int main() {
    // Basic test from description
    // ram_size = 64, min_block_size = 4
    BuddyAllocator ba(64, 4);

    // malloc(8) should return 0
    int a1 = ba.malloc(8);
    std::cout << "malloc(8) -> " << a1 << std::endl;
    assert(a1 == 0);

    // malloc(8) should return 8
    int a2 = ba.malloc(8);
    std::cout << "malloc(8) -> " << a2 << std::endl;
    assert(a2 == 8);

    // malloc_at(16, 8) should return 16
    int a3 = ba.malloc_at(16, 8);
    std::cout << "malloc_at(16, 8) -> " << a3 << std::endl;
    assert(a3 == 16);

    // free_at(8, 8)
    ba.free_at(8, 8);
    std::cout << "free_at(8, 8)" << std::endl;

    // malloc(8) should return 8 (reusing freed block)
    int a4 = ba.malloc(8);
    std::cout << "malloc(8) -> " << a4 << std::endl;
    assert(a4 == 8);

    // malloc(32) should return 32
    int a5 = ba.malloc(32);
    std::cout << "malloc(32) -> " << a5 << std::endl;
    assert(a5 == 32);

    // Try to malloc something too large
    int a6 = ba.malloc(64);
    std::cout << "malloc(64) -> " << a6 << std::endl;
    assert(a6 == -1);

    std::cout << "All basic tests passed!" << std::endl;

    return 0;
}
