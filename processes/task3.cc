#include <iostream>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        return -1;
    }
    execve(argv[1], argv + 1, nullptr);
}