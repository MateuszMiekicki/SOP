#include <fstream>
#include <iostream>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        return -1;
    }
    auto file = std::ifstream(argv[1]);
    std::cout << file.rdbuf();
}