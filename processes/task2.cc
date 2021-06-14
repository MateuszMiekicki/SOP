#include <iostream>
#include <unistd.h>

int main()
{
    enum
    {
        ERROR = -1,
        CHILD_PROCESS = 0
    };
    switch (auto pid = fork(); pid)
    {
    case ERROR:
    {
        return -1;
    }
    break;
    case CHILD_PROCESS:
    {
        std::cout << "Parent pid: " << getppid() << '\n';
    }
    break;
    default:
    {
        std::cout << "Child pid: " << pid << '\n';
    }
    break;
    }
}