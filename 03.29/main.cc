#include <cstdlib>
#include <iostream>

template <typename T>
struct arena
{
    T *ptr = nullptr;
    arena()
    {
        ptr = static_cast<T *>(malloc(100000000));
    }
    arena(const std::size_t size)
    {
        ptr = malloc(size);
    }
    std::size_t currnet = 0;
    void *allocate(const std::size_t n)
    {
        currnet += n;
        return ptr + currnet - n;
    }
};
int main()
{
    {
        arena<int> a;
        int *n = static_cast<int *>(a.allocate(10));
        for (int i = 0; i < 10; ++i)
        {
            *(n + i) = i;
        }
        for (int i = 0; i < 10; ++i)
        {
            std::cout << *(n + i) << '\n';
        }
    }
}