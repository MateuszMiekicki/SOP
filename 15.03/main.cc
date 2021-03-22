#include <sys/stat.h>
#include <fcntl.h>
#include <string>
#include <unistd.h>
#include <memory>
#include <iostream>
#include <stdexcept>

class file
{
private:
    std::string _nameFile;
    int _fileDescriptors;
    std::string _content;
    void openFile()
    {
        _fileDescriptors = open(_nameFile.c_str(), O_RDONLY);
        if (-1 == _fileDescriptors)
        {
            throw std::invalid_argument("file does not exist");
        }
    }
    void readFile()
    {
        struct stat stat_buf;
        fstat(_fileDescriptors, &stat_buf);
        auto buff = std::make_unique<char[]>(stat_buf.st_size);
        read(_fileDescriptors, buff.get(), stat_buf.st_size);
        _content = buff.get();
    }

public:
    file() = default;
    file(const std::string &nameFile) : _nameFile{nameFile}
    {
        openFile();
        readFile();
    }
    file(const file &file) = delete;
    file(file &&file) = default;
    file &operator=(const file &file) = default;
    file &operator=(file &&file) = delete;
    std::ostream &operator<<(std::ostream &os)
    {
        os << _content;
        return os;
    }
    ~file()
    {
        close(_fileDescriptors);
    }
};

int main(int argc, char **argv)
{
    for (auto i{1}; i < argc; ++i)
    {
        file f(argv[i]);
        f << std::cout;
    }
}