#include "../include/network/Server.hpp"



int main(int argc, char** argv)
{
    if (argc < 3 || argc >= 4)
    {
        std::cout << "./file port pass" << std::endl;
        return 1;
    }
    Server server(argv[1], argv[2]);

    
    std::cout << "work" << std::endl;
}