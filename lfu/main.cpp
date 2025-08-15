#include <iostream>
#include "lfu_cache.hpp"

int main() {
    size_t cacheSize, elemCount;
    std::cin >> cacheSize >> elemCount;

    lfu::lfu_cache_t<int, int> cache{cacheSize};

    std::cout << "Cache capacity: " << cache.capacity_ << std::endl;

    for (int i = 0; i < elemCount; ++i) {
        int elem;
        std::cin >> elem;
        cache.lookupUpdate(elem, elem);
    }

    std::cout << "Hits getted: " << cache.retHits() << std::endl;

    return 0;
}