#include <stdio.h>
#include <sys/mman.h>

int main()
{
    auto *ptr = reinterpret_cast<int *>(mmap(nullptr,
                                             4096,
                                             PROT_READ | PROT_WRITE,
                                             MAP_PRIVATE | MAP_ANONYMOUS,
                                             0,
                                             0));

    if (ptr == MAP_FAILED)
    {
        printf("Mapping Failed\n");
        return 1;
    }
    int err = munmap(ptr, 4096);
    if (err != 0)
    {
        printf("UnMapping Failed\n");
        return 1;
    }
}