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
    auto value = std::stoi({argv[2]});
    std::cout << pid << ' ' << value;
    union sigval sv;
    sv.sival_int = value;
    if (sigqueue(pid, SIGUSR1, sv) != 0)
    {
        perror("SIGSENT-ERROR:");
    }
}