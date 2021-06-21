#include <cstdint>
#include <iostream>

int main()
{
    uint32_t ptr = 0x12345678;

    const uint32_t PAGE_OFFSET_MASK{0x00000FFF};
    uint32_t pageOffset = (ptr & PAGE_OFFSET_MASK) >> 0;
    std::cout << std::hex << pageOffset << '\n';

    const uint32_t SECOND_OFFSET_MASK{0x003FF000};
    uint32_t secondLevelPage = (ptr & SECOND_OFFSET_MASK) >> 12;
    std::cout << std::hex << secondLevelPage << '\n';

    const uint32_t FIRST_OFFSET_MASK{0xFFC00000};
    uint32_t firstLevelPage = (ptr & FIRST_OFFSET_MASK) >> 22;
    std::cout << std::hex << firstLevelPage << '\n';

    return 0;
}