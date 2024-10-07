#include "../include/network/Server.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <cstring>



int main(int argc, char** argv)
{
    if (argc < 3 || argc >= 4)
    {
        std::cout << "./file port pass" << std::endl;
        return 1;
    }
    Server server(argv[1], argv[2]);
    server.start();

    
    std::cout << "work" << std::endl;
}

