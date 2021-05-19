 #include <cstdlib>
#include <iostream>

template <typename T>
struct arena
{
    T *ptr = nullptr;
    arena()
    {
        ptr = static_cast<T *>(malloc((1'000'000'000 / sizeof(T)) * sizeof(T)));
    }
    arena(const std::size_t size)
    {
        ptr = static_cast<T *> malloc((size / sizeof(T)) * sizeof(T));
    }
    std::size_t currnet = 0;
    T *allocate(const std::size_t n)
    {
        currnet += n;
        return ptr + currnet - n;
    }

    void deallocate(const std::size_t n)
    {
    }

    ~arena()
    {
        free(ptr);
    }
};
int main()
{
    {
        arena<int> a;
        int *n = a.allocate(10);
        for (int i = 0; i < 10; ++i)
        {
            *(n + i) = i;
        }
        for (int i = 0; i < 10; ++i)
        {
            std::cout << *(n + i) << '\n';
        }

    }

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
    }
}