#include <arpa/inet.h>
#include <endian.h>
#include <iostream>
#include <netinet/ip.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <stdexcept>
#include <cstring>
#include <thread>
#include <vector>
#include <mutex>

class Server
{
private:
    std::string ipAddress_;
    std::uint16_t port_;
    int serverFileDescriptor_;

public:
    Server() : ipAddress_{""},
               port_{0},
               serverFileDescriptor_{-1}
    {
    }
    void setIp(const std::string &ipAddress)
    {
        ipAddress_ = ipAddress;
    }

    void setPort(const std::uint16_t port)
    {
        port_ = port;
    }

    void connect(const std::string &ipAddress, const std::uint16_t port)
    {
        ipAddress_ = ipAddress;
        port_ = port;
        connect();
    }

    void connect()
    {
        sockaddr_in sockAddress;
        memset(&sockAddress, 0, sizeof(sockAddress));
        sockAddress.sin_family = AF_INET;
        sockAddress.sin_port = htobe16(port_);
        inet_pton(sockAddress.sin_family, ipAddress_.c_str(), &sockAddress.sin_addr);
        serverFileDescriptor_ = socket(AF_INET, SOCK_STREAM, 0);
        if (bind(serverFileDescriptor_, reinterpret_cast<sockaddr *>(&sockAddress), sizeof(sockAddress)) not_eq 0)
        {
            disconect();
            throw std::logic_error("can not bind sock");
        }
        listen(serverFileDescriptor_, 0);
    }

    void disconect()
    {
        if (serverFileDescriptor_ not_eq -1)
        {
            shutdown(serverFileDescriptor_, SHUT_RDWR);
            close(serverFileDescriptor_);
        }
    }

    int getFileDescriptor() const
    {
        return serverFileDescriptor_;
    }

    ~Server()
    {
        disconect();
    }
};

class Client
{
private:
    int clientFileDescriptor_;

public:
    Client(const int clientFileDescriptor)
    {
        if (clientFileDescriptor == -1)
        {
            throw std::logic_error("cannot accept request from client");
        }
        clientFileDescriptor_ = clientFileDescriptor;
    }
    Client() : clientFileDescriptor_{-1}
    {
    }
    Client(Client &&client) : clientFileDescriptor_{client.clientFileDescriptor_}
    {
    }

    Client &operator=(Client &&client) noexcept
    {
        std::swap(clientFileDescriptor_, client.clientFileDescriptor_);
        return *this;
    }

    void read()
    {
        char buffer[1024];
        do
        {
            memset(&buffer, 0, sizeof(buffer));
            int result = ::read(clientFileDescriptor_, buffer, 1024);
            if (result < 1)
            {
                perror("result<1");
                disconect();
                return;
            }
            std::cout << buffer << "\n";
        } while (strcmp("shutdown\n", buffer) not_eq 0);
        disconect();
    }
    int getClientFileDescripor() const
    {
        return clientFileDescriptor_;
    }

    void disconect()
    {
        if (clientFileDescriptor_ not_eq -1)
        {
            std::cout << "client " << clientFileDescriptor_ << " disconnected\n";
            shutdown(clientFileDescriptor_, SHUT_RDWR);
            close(clientFileDescriptor_);
        }
    }
    ~Client()
    {
        disconect();
    }
};

std::vector<std::pair<Client, std::pair<bool, std::thread>>> clients;
std::mutex clientsMutex;

class Listener
{
private:
    int serverFileDescriptor_;
    std::thread threadForListing;

public:
    Listener(const int serverFileDescriptor) : serverFileDescriptor_{serverFileDescriptor}
    {
    }
    void listen()
    {
        while (true)
        {
            auto newClientFileDescriptor = accept(serverFileDescriptor_, nullptr, nullptr);
            if (newClientFileDescriptor not_eq -1)
            {
                const std::lock_guard<std::mutex> lock(clientsMutex);
                std::thread th;
                clients.emplace_back(newClientFileDescriptor, std::make_pair(false, std::move(th)));
            }
        }
    }
    void clen()
    {
        const std::lock_guard<std::mutex> lock(clientsMutex);
        for (int i = 0; i < clients.size(); ++i)
        {
            if (clients.at(i).first.getClientFileDescripor() == -1)
            {
                clients.at(i) = std::move(clients.back());
                clients.pop_back();
            }
        }
    }
};

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cerr << "you must pass the ip address and port of server";
        return -1;
    }
    auto serverAddres = argv[1];
    auto serverPort = static_cast<uint16_t>(std::stoi(argv[2]));

    Server server;
    server.connect(serverAddres, serverPort);
    //Note: reserve disconnects all clients
    //list will be a good choice. Maybe a map for a constant search
    clients.reserve(100);
    Listener listener(server.getFileDescriptor());
    std::thread listing(&Listener::listen, std::ref(listener));
    listing.detach();
    auto cleanEverySecond = [&listener]()
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        listener.clen();
    };
    std::thread cleaner(cleanEverySecond);
    cleaner.detach();
    while (true)
    {
        const std::lock_guard<std::mutex> lock(clientsMutex);

        for (auto &[client, thread] : clients)
        {
            if (!thread.first)
            {
                thread.second = std::thread(&Client::read, std::ref(client));
                thread.second.detach();
                thread.first = true;
            }
        }
    }
}