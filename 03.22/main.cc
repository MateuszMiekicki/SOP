#include <unistd.h>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

int main(int argc, char *argv[])
{
    {
        auto pointer = 0xdeadbeef;
        //auto value = *reinterpret_cast<int *>(pointer);
    }
    {
        if (argc > 1)
        {
            const std::vector<const char *> cmdline{argv + 1, argv + argc};
            errno = 0;
            execvp(argv[1], const_cast<char *const *>(cmdline.data()));
        }
    }
    {
    }
}