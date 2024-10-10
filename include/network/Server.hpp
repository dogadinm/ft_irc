#pragma once


#define NI_MAXSERV    32
#define NI_MAXHOST  1025

#include <sys/socket.h>
#include <netdb.h>
#include <sys/poll.h>
#include <poll.h>
#include <iostream>
#include <stdexcept>
#include <fcntl.h>
#include <string> 


#include <algorithm>
#include <vector>
#include <map>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <cstring>

// #include "Client.hpp"
// #include "Channel.hpp"

class Server
{
     typedef std::vector<pollfd>::iterator       plfds_iterator;
private:
    int _working;
    int _socket;

    const std::string _port;
    const std::string _pass;
    const std::string _host;

    std::vector<pollfd> _plfds;
    // std::vector<Channel *>  _channels;
    std::map<int, uint16_t> _clients;

    Server();
    Server(Server const & copy);
    
public:

    // Constructor and Destructor
    Server(const std::string &port, const std::string &pass);
    ~Server();

    // Start server
    void start();


    int CreateSocket();

    // Getters
    std::string get_pass() const;
    
    // Handle Clients
    void            on_client_connect();
    void            on_client_disconnect(int fd);
    void            on_client_message(int fd);

    std::string     read_message(int fd);
};

