#include <chrono>
#include <iostream>
#include <random>
#include <sys/wait.h>
#include <thread>
#include <unistd.h>

unsigned randomNaturalNumber(const unsigned max)
{
    const unsigned min{1};
    std::random_device random;
    std::mt19937 gen(random());
    std::uniform_int_distribution<> randomNumber(min, max);
    return randomNumber(gen);
}

int returnRandomValue()
{
    return randomNaturalNumber(10) % 2;
}

void createChild()
{
    enum
    {
        ERROR = -1,
        CHILD_PROCESS = 0
    };
    const unsigned MAX_SLEEP_TIME{5};
    switch (auto child = fork(); child)
    {
    case ERROR:
    {
        throw;
    }
    break;
    case CHILD_PROCESS:
    {
        std::this_thread::sleep_for(std::chrono::seconds(randomNaturalNumber(MAX_SLEEP_TIME)));
        exit(returnRandomValue());
        return;
    }
    break;
    default:
    {
        return;
    }
    break;
    }
}

bool checkChildStatus()
{
    int status;
    pid_t cpid = waitpid(-1, &status, 0);
    printf("Child %d: returned value is: %d\n", cpid, WEXITSTATUS(status));
    return WEXITSTATUS(status);
}

int main()
{
    const auto NUMBER_OF_CHILD{4};
    for (auto i{0}; i < NUMBER_OF_CHILD; ++i)
    {
        createChild();
    }
    auto counter{4};
    while (counter)
    {
        if (checkChildStatus())
        {
            --counter;
        }
        {
            createChild();
        }
    }
}
