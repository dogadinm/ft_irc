#pragma once

#include <sys/socket.h>
#include <iostream>


#include <algorithm>
#include <vector>
#include <map>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <cstring>

class Server
{
private:
    int _working;
    int _socket;
    const std::string _port;
    const std::string _pass;
    const std::string _host;

    
public:
    Server(const std::string &port, const std::string &pass);
    ~Server();
    int CreateSocket();
    void start();
};

Server::Server(const std::string &port, const std::string &pass) : _host("127.0.0.1"), _port(port), _pass(pass)
{
    _working = 1;
    _socket = CreateSocket();
}

int Server::CreateSocket()
{
    int server_fd;
    struct sockaddr_in server_addr;
    int port = std::stoi(_port);
    

    // Create the server socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        return 1;
    }

    // Set up the server address struct
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET; // Set the address family to IPv4
    server_addr.sin_addr.s_addr = INADDR_ANY; // Bind to all available interfaces
    server_addr.sin_port = htons(port); // Convert the port to network byte order

    // allow port reuse
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    // Bind the socket to the address and port
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        close(server_fd);
        return 1;
    }

    // Listen for incoming connections
    if (listen(server_fd, 5) < 0) {
        perror("listen");
        close(server_fd);
        return 1;
    }
    std::cout << "Server listening on port " << port << std::endl;
    return server_fd;

    
}

void            Server::start()
{
    int client_fd;
    struct sockaddr_in  client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    char buffer[1024];

    while (_working)
    {
        // Accept a connection
        client_fd = accept(_socket, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_fd < 0) {
            perror("accept");
            close(_socket);
        }
    
        std::cout << "Client connected" << std::endl;
        while (true){

            memset(buffer, 0, sizeof(buffer));

            // Read data from the client
            ssize_t bytes_read = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
            if (bytes_read < 0) {
                perror("read");
                close(client_fd);
                close(_socket);
            }

            if (bytes_read == 0) {
                // Клиент закрыл соединение
                std::cout << "Client disconnected" << std::endl;
                break;
            }


            buffer[bytes_read] = '\0'; // Null-terminate the buffer
            std::string receivedMessage(buffer);
            std::cout << "Received message: " << buffer << std::endl;
            
            receivedMessage.erase(std::remove_if(receivedMessage.begin(), receivedMessage.end(), ::isspace),
                receivedMessage.end());

            if (receivedMessage == "QUIT")
            {
                _working = 0;
                std::cout << "Client requested to quit." << std::endl;
                break;
            }

            // std::string response = "Server received: " + std::string(buffer);
            // send(client_fd, response.c_str(), response.length(), 0);
            

            
        }
        // Close the client and server sockets
        close(client_fd);
    }
    close(_socket);

}

Server::~Server()
{
    if (_socket >= 0) {
        close(_socket);
    }
}
