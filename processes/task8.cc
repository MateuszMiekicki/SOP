#include <iostream>
#include <signal.h>
#include <string.h>
#include <string>

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        return -1;
    }
    auto pid = std::stoi({argv[1]});
    auto signal = std::stoi({argv[2]});
    if (kill(pid, signal) != 0)
    {
        perror("SIGSENT-ERROR:");
    }
}